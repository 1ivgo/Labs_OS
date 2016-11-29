#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
 	struct timeval tv;

 	unsigned long long t1,t2,t3,tr;

	gettimeofday(&tv, NULL); 
	t1 = (unsigned long long)tv.tv_sec*1000000 + tv.tv_usec;
	if (fork()>0)
	{
		
		gettimeofday(&tv, NULL); 
		t2 = (unsigned long long)tv.tv_sec*1000000 + tv.tv_usec;
		tr = t2-t1;
		printf("Запустился родитель, время: %llu микросекунд\n",tr);
	} else 
	{
		
		gettimeofday(&tv, NULL); 
		t3 = (unsigned long long)tv.tv_sec*1000000 + tv.tv_usec;
		tr = t3-t1;
		printf("Запустился ребенок, время: %llu микросекунд\n",tr);
	}
	return 0;
}
