#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void numbers_of_Fib(){
	long double Fib[100];
	Fib[0]=1;
	Fib[1]=1;
	int i;
	for (i=2; i<100; i++){
		Fib[i] = Fib[i-2] + Fib[i-1];
	}
	for (i=0; i<100; i++){
		printf("%Lf\n", Fib[i]);
	}
}

void* thread_func(void * arg){
  numbers_of_Fib();
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

