#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* thread_func(void * arg){
	long double Fib[100];
	Fib[0]=1;
	Fib[1]=1;
	int i;
	for (i=2; i<99; i++){
		Fib[i] = Fib[i-2] + Fib[i-1];
	}
	
	for (i=0; i<99; i++){
		printf("%Thread: %x\t .Lf\n", Fib[i]);
	}
	
  pthread_exit(0);
}

int main()
{
	pthread_t thread[10];
	int i;
	for(i=0; i<10; i++){
		pthread_create(&thread[i], NULL, thread_func, NULL);
	}
	
	for(i=0; i<10;i++){
		pthread_join( thread[i], NULL);
	}
	
	return 0;
}
