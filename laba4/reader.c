#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

int main ()
{
	key_t key;
	int fd, shmid, i, len_str;
	char *shm, str_writer[300], str_reader[300], str_FIFO[30];
	time_t cur_time;
	struct tm * tv;

	//создаем именованный канал (FIFO)
	
	if ((fd = open("File", O_RDONLY)) < 0){
		printf("Ошибка открытия FIFO файла\n");
		exit(0);
	}
	//создаем ключ
	if ((key = ftok("shar_mem_file", 0)) < 0){
		printf("Ошибка создания ключа\n");
		exit(0);
	}
	//создаем сегмент разделяемой памяти
	if ((shmid = shmget(key, 50, IPC_CREAT | IPC_EXCL | 0777)) < 0){
		if (errno == EEXIST){		
			if ((shmid = shmget(key, 50,  0777)) < 0){
				printf("Ошибка создания разделяемой памяти\n");
				exit(0);
			}
		}else{
			printf("Ошибка создания разделяемой памяти\n");
			exit(0);
		}
	}
	if ((shm = (char*)shmat(shmid, NULL, 0)) == (char*)-1){
		printf("Ошибка подстыковки куска разделяемой памяти\n");
		exit(0);
	}

	bzero(shm, 50);

	for(i=0; i<10; i++){
		while(read(fd, str_FIFO, 29)<=0)
			sleep(1);

		printf("FIFO reader: %s", str_FIFO);
		memcpy(str_writer, shm, strlen(shm));

		cur_time = time(NULL);
		tv = localtime(&cur_time);
		sprintf (str_reader, "Reader:%x %s\n", getpid(), asctime(tv));
		memcpy(shm, str_reader, strlen(str_reader));
		
		printf("Shared memory: %s\t%s", str_writer, str_reader);
		sleep(1);
	}
	return 0;
}
