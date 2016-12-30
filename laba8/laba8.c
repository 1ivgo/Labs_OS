#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include  <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>
#include <unistd.h>

pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex;
int num_str=1, flag=0;

void* thread_func_writer(void * arg){
	
	char str[30];
	time_t cur_time;
	struct tm * tv;
	int i, try_lock, fd;
	int len_str;
	
	while(num_str<10){
		
		pthread_mutex_lock(&mutex);

		if ((fd = open("./File.txt", O_RDWR | O_CREAT, 0666)) == -1){
			printf("Ошибка открытия файла\n");
			exit(0);
		}
		
		cur_time = time(NULL);
		tv = localtime(&cur_time);
		len_str = sprintf (str, "%d\t %s", num_str, asctime(tv));
		write(fd, str, len_str);
		num_str++;
		
		flag = 1;
		pthread_cond_signal(&cond_var);
		pthread_mutex_unlock(&mutex);

		sleep(1);
		close(fd);
		
	}
	pthread_exit(0);
}

void* thread_func_reader(void* arg){
	
	int fd, buffer_read=0, buffer_written=0, i;
	char buffer[30];
	
	for(i=0; i<10; i++){

		pthread_mutex_lock(&mutex);
		while (flag != 1){ 
      		pthread_cond_wait(&cond_var, &mutex);
      	}

		if ((fd = open("./File.txt", O_CREAT|O_RDONLY, 0666)) == -1){
			printf("Ошибка открытия файла\n");
			exit(0);
		}
		
		while ((buffer_read = read(fd, buffer, 30)) > 0){
			buffer_written = write (1, buffer, buffer_read);
			if (buffer_written != buffer_read)
				printf("Ошибка чтения");
		}

		pthread_mutex_unlock(&mutex);

		sleep(1);
	}
	flag = 0;
	pthread_exit(0);
}

int main()
{
	
	int i;
	
	pthread_t thread_writer;
	pthread_t thread_reader[10];
	


	if(pthread_create(&thread_writer, NULL, thread_func_writer, NULL)<0)
		printf("Ошибка создания писателя\n");
	
	for(i=0; i<10; i++){
		if(pthread_create(&thread_reader[i], NULL, thread_func_reader, NULL)<0)
			printf("Ошибка создания читателя\n");
	}

	pthread_join(thread_writer, NULL);
	for(i=0; i<10;i++){
		pthread_join(thread_reader[i], NULL);
	}

	
	
	return 0;
}