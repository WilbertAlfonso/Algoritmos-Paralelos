#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
double timeval_diff(struct timeval *a, struct timeval *b)
{
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}
void llenarM(double *matrix,int fila, int columna)
{
    for (int i=0;i<(fila*columna);i++) 
    {
		*(matrix + i)=1 + (double)(10*random()/(RAND_MAX+1.0));
    }

}
int mostrarM(double *matriz, int fila, int columna)
{
    for (int x = 0; x < fila; x++) 
    {
		for (int y = 0; y < columna; y++) 
		{
	    	printf("%7g ",matriz[x*columna+y]);
		}
		printf("\n");
    }
    return 0;
}

int thread_count;
int filas=8;
int columnas=8000000;
double* Matriz;
double* Vector;
double* Respuesta;
void* MulMV(void * rango)
{
	long mi_rango=(long) rango;
	int seccion=filas/thread_count;
	int inicio=mi_rango*seccion;
	int fin=(mi_rango+1)*seccion-1;
	for (int i = inicio; i <= fin; i++)
	{
		Respuesta[i]=0.0;
		for (int j = 0; j < columnas; j++)
		{
			Respuesta[i]+=Matriz[i*columnas+j]*Vector[j];
		}
	}
	return NULL;
}
int main(int argc,char* argv[])
{

	long thread;
	pthread_t* thread_handles;
	thread_count=strtol(argv[1],NULL,10);
	thread_handles=malloc (thread_count*sizeof(pthread_t));
	Matriz=malloc(filas*columnas*sizeof(double));
	Vector=malloc(columnas*sizeof(double));
	Respuesta=malloc(filas*sizeof(double));
	llenarM(Matriz,filas,columnas);
	llenarM(Vector,1,columnas);
	printf("\n");
	struct timeval t_ini, t_fin;
  	double secs;
  	gettimeofday(&t_ini, NULL);
	for(thread=0;thread<thread_count;thread++)
	{
		pthread_create(&thread_handles[thread],NULL,MulMV,(void *)thread);
	}
	for(thread=0;thread<thread_count;thread++)
	{
		pthread_join(thread_handles[thread],NULL);
	}
	gettimeofday(&t_fin, NULL);
  	secs = timeval_diff(&t_fin, &t_ini);
  	printf("%.12g milliseconds\n", secs);
	free(thread_handles);
	free(Matriz);
	free(Vector);
	free(Respuesta);
	return 0;
}