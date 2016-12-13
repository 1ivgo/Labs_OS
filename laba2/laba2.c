#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 100

int main() 
{ 
	int fd;
	char buf[BUF_SIZE];
	char buf2[BUF_SIZE]; 
	ssize_t num_written;
	ssize_t num_readen;
	size_t buf_off_set = 0;
	pid_t Child_pid;

	if ((fd = open("./File", O_RDWR | O_CREAT, 0666)) == -1)
	{
		printf("Ошибка открытия файла\n");
		return -1;
	}

	Child_pid = fork();

	if(Child_pid < 0)
	{
		printf("Ошибка создания процесса\n");
	}

	int st;
	if (Child_pid > 0)
	{
		wait(&st);
	
		size_t buf_size = sprintf(buf2, "Я родитель, мой пид: %d\n", getpid());
		
		do
		{
			num_written = write(fd, buf2, buf_size - buf_off_set);

			if (num_written == -1)
			{
				printf("Ошибка записи\n");
				return -1;
			}

			buf_off_set += num_written;

		}
		while (buf_off_set != buf_size);
		
		lseek(fd, 0, SEEK_SET);
		while (num_readen = read(fd, buf, BUF_SIZE) > 0)
		{
			if (num_readen == -1)
			{
				printf("Ошибка чтения\n");
				return -1;
			}
			
			buf_off_set += num_readen;
		}
		num_readen = 0;
		printf("%s", buf);
		
		close(fd);
	}
	else
	{
		size_t buf_size = sprintf(buf, "Я ребенок, мой пид: %d\n", getpid());
	
		do
		{
			num_written = write(fd, buf, buf_size - buf_off_set);

			if (num_written == -1)
			{
				printf("Ошибка записи\n");
				return -1;
			}

			buf_off_set += num_written;

		}
		while (buf_off_set != buf_size);
	}
	return 0;
}
