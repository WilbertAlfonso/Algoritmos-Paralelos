#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Hello(void);
void mostrar(float matriz[3][4],int fila,int columna)
{
	for (int i = 0; i < fila; i++)
	{
		for (int j = 0; j < columna; j++)
		{
			printf("%7g ",matriz[i][j]);
		}
		printf("\n");
	}
}
int main(int argc,char* argv[])
{
	int filas=3;
	int columnas=filas+1;
	float Matriz[3][4]={{2,1,-1,8},{-3,-1,2,-11},{-2,1,2,-3}};
	int thread_count=strtol(argv[1],NULL,10);
	#pragma omp parallel num_threads(thread_count)
	Hello();
	for (int i = 0; i < filas; i++)
	{
		#pragma omp parallel for num_threads(thread_count)\
			schedule(static,1)
		for(int j=0;j<filas;j++)
		{
			if(j!=i)
			{
				float temp=Matriz[j][i]; 
				for(int k=0;k<columnas;k++)
				{
					Matriz[j][k]-=Matriz[i][k]*temp/Matriz[i][i];
				}
			}
		}
	}
	mostrar(Matriz,filas,columnas);
	return 0;
}
void Hello(void)
{
	int my_rank=omp_get_thread_num();
	int thread_count=omp_get_num_threads();
	printf("Hello from %d of %d \n",my_rank,thread_count );
}