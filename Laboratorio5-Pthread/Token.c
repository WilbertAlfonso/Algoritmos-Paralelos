#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <semaphore.h>
double timeval_diff(struct timeval *a, struct timeval *b)
{
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}
int thread_count;
sem_t* sems;
FILE *stdin;
char * puntero;
void* Token(void* rango)
{
	long mi_rango = (long) rango;
	int contador;
	int siguiente=(mi_rango+1)%thread_count;
	char* documento;
	char buffer[500];
	char* nuevo;
	sem_wait(&sems[mi_rango]);
	documento=fgets(buffer,500,stdin);
	sem_post(&sems[siguiente]);
	while(documento!=NULL)
	{
		printf("Thread %ld > mi seccion = %s",mi_rango,buffer);
		contador=0;
		nuevo=strtok_r(buffer," \t\n",&puntero);
		//nuevo=strtok(buffer, " \t\n");
		while(nuevo != NULL)
		{
			contador++;
			printf("Thread %ld > cadena %d = %s\n",mi_rango,contador,nuevo);
			//nuevo=strtok(NULL," \t\n");
			nuevo=strtok_r(NULL," \t\n",&puntero);
		}
		sem_wait(&sems[mi_rango]);
		documento=fgets(buffer,500,stdin);
		sem_post(&sems[siguiente]);
	}
	return NULL;
}
int main(int argc,char* argv[])
{
	stdin=fopen("file.txt","r");
	long thread;
	pthread_t* thread_handles;
	thread_count=strtol(argv[1],NULL,10);
	thread_handles=malloc (thread_count*sizeof(pthread_t));
	sems=malloc(thread_count*sizeof(sem_t));
	printf("\n");
	struct timeval t_ini, t_fin;
  	double secs;
  	gettimeofday(&t_ini, NULL);
  	sem_post(&sems[0]);
	for(thread=0;thread<thread_count;thread++)
	{
		pthread_create(&thread_handles[thread],NULL,Token,(void *)thread);
	}
	for(thread=0;thread<thread_count;thread++)
	{
		pthread_join(thread_handles[thread],NULL);
	}
	gettimeofday(&t_fin, NULL);
  	secs = timeval_diff(&t_fin, &t_ini);
  	printf("%.16g milliseconds\n", secs * 1000.0);
	free(thread_handles);
	return 0;
}