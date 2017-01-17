#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex;
int num_str=1, flag=0;

void* thread_func_writer(void * arg){
	
	char str[30];
	time_t cur_time;
	struct tm * tv;
	int i, try_lock, fd;
	int len_str;
	
	if ((fd = open("./File.txt", O_RDWR | O_CREAT, 0744)) == -1){
			printf("Ошибка открытия файла\n");
			exit(0);
	}

	while(1){
		pthread_mutex_lock(&mutex);

		cur_time = time(NULL);
		tv = localtime(&cur_time);
		len_str = sprintf (str, "%d\t %s", num_str, asctime(tv));
		write(fd, str, len_str);
		
		flag = 1;
		num_str++;
		
		pthread_cond_signal(&cond_var);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	close(fd);
	pthread_exit(0);
}

void* thread_func_reader(void* arg){
	
	int fd, buffer_read=0, buffer_written=0, i;
	char buffer[30];
	
	if ((fd = open("./File.txt", O_CREAT|O_RDONLY, 0744)) == -1){
		printf("Ошибка открытия файла\n");
		exit(0);
	}	
	while(1){
		pthread_mutex_lock(&mutex);
		while (flag != 1){ 
	    	pthread_cond_wait(&cond_var, &mutex);
	    }

		printf("Thread: %lx:\n", pthread_self());

		while ((buffer_read = read(fd, buffer, 30)) > 0){
			buffer_written = write(1, buffer, buffer_read);
			if (buffer_written != buffer_read)
				printf("Ошибка чтения");
		}
		flag = 0;
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
	close(fd);
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