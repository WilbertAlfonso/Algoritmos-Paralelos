#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>
double timeval_diff(struct timeval *a, struct timeval *b)
{
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}
int thread_count;
double sum_global;
int a=0;
int b=100;
int n=5000;
double h;
pthread_mutex_t mutex;
sem_t   sem; 
double funcion(double x) {
    double return_val;

    return_val = (x*x)-2;
    return return_val;
}

void*  Trap(void* rank)
{   
	long rango=(long) rank;
	int particion=(n-2)/thread_count;
	int inicio=rango*particion;
	int fin=inicio+particion;
	printf("inicio = %d\n fin =%d\n ", inicio,fin);
    for (double i = a+h+rango*particion*h, ite=0; ite <= particion; ite++,i+=h) {
    	pthread_mutex_lock(&mutex);
        //sem_wait(&sem);
        sum_global += funcion(i);
        //sem_post(&sem);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main(int argc,char* argv[])
{
	h=(b-a)/n;
	long thread;
	pthread_t* thread_handles;
	thread_count=strtol(argv[1],NULL,10);
	thread_handles=malloc (thread_count*sizeof(pthread_t));
	sum_global+=(funcion(a)+funcion(b))/2;
	struct timeval t_ini, t_fin;
  	double secs;
  	gettimeofday(&t_ini, NULL);
  	sem_init(&sem, 0, 1);
	for(thread=0;thread<thread_count;thread++)
	{
		pthread_create(&thread_handles[thread],NULL,Trap,(void *)thread);
	}
	for(thread=0;thread<thread_count;thread++)
	{
		pthread_join(thread_handles[thread],NULL);
	}
	printf("Integral = %g\n",sum_global*h);
	gettimeofday(&t_fin, NULL);
  	secs = timeval_diff(&t_fin, &t_ini);
  	printf("%.12g milliseconds\n", secs);
	free(thread_handles);
	return 0;
}