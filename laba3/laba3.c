#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* numbers_of_Fib(void*){
	long double Fib[100];
	Fib[0]=1;
	Fib[1]=1;
	int i, k=0;
	for (i=2; i<100; i++){
		Fib[i] = Fib[i-2] + Fib[i-1];
		printf("%Lf\n", Fib[k]);
		k++;
	}
}

int main()
{
	pthread_t thread[10];
	//pthread_t thread;	
	//numbers_of_Fib();
	int i;
	for(i=0; i<10; i++){
		pthread_create( &thread[i], NULL, numbers_of_Fib, NULL );
	}
	
	for(i=0; i<10;i++){
		pthread_join( thread[i], NULL);
	}
	
	return 0;
}

