#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
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

void Mat_vect_mult(
	double local_A[],
    double local_x[],
    double local_y[],
    int local_m,
    int n,
    int local_n)
{
    double * x;
    int local_i,j;
    int local_ok=1;
    x=malloc(n*sizeof(double));
    MPI_Allgather(local_x,local_n,MPI_DOUBLE,x,local_n,MPI_DOUBLE,MPI_COMM_WORLD);
    for(local_i=0;local_i<local_m;local_i++)
    {
    	local_y[local_i]=0.0;
    	for(j=0;j<n;j++)
    	{
    		local_y[local_i]+=local_A[local_i*n+j]*x[j];
    	}
    }
    free(x);
}
int main(int argc, char **argv)
{
	double start_time, end_time;
    int my_rank, comm_sz;
    double * Matriz;
	double * Vector;
	double * Resultado;
	double * Resultados;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int filas=4096;
	int columnas=4096;
	Matriz=malloc(filas*columnas*sizeof(double));
    Vector=malloc(columnas*sizeof(double));
    Resultados=malloc(filas*sizeof(double));
    if(my_rank==0)
    {
	    llenarM(Matriz,filas,columnas);
	    llenarM(Vector,1,columnas);
	    MPI_Bcast(Matriz,filas*columnas,MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Bcast(Vector,columnas,MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    else
    {
    	MPI_Bcast(Matriz,filas*columnas,MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Bcast(Vector,columnas,MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    
	double* MiMat;
    double* MiVec;
    int inicioM,finM,saltoM,inicioV,finV,saltoV;
	saltoM = (int)(filas/comm_sz);
	inicioM = saltoM*(my_rank);
	finM = inicioM+saltoM-1;
	if (my_rank == comm_sz-1) 
	{
	    finM =finM+(filas%comm_sz);
	}
	saltoV = (int)(columnas/comm_sz);
	inicioV = saltoV*(my_rank);
	finV = inicioV+saltoV-1;
	if (my_rank == comm_sz-1) 
	{
	    finV =finV+(columnas%comm_sz);
	}
	MiMat=malloc((finM-inicioM+1)*columnas*sizeof(double));
	MiVec=malloc((finV-inicioV+1)*sizeof(double));
	Resultado=malloc((finM-inicioM+1)*sizeof(double));
	for (int i=inicioM,k=0; i <=finM; i++,k++)
	{
		for (int j=0; j<columnas;j++)
		{
			MiMat[k*columnas+j]=Matriz[i*columnas+j];
		}
	}
	for (int i = inicioV,j=0; i <=finV; i++,j++)
	{
		MiVec[j]=Vector[i];
	}
	free(Matriz);
	free(Vector);
	MPI_Barrier(MPI_COMM_WORLD);
	start_time = MPI_Wtime();
	Mat_vect_mult(MiMat,MiVec,Resultado,finM-inicioM+1,columnas,finV-inicioV+1);
	MPI_Barrier(MPI_COMM_WORLD); 
	end_time= MPI_Wtime();
	MPI_Allgather(Resultado,finM-inicioM+1,MPI_DOUBLE,Resultados,finM-inicioM+1,MPI_DOUBLE,MPI_COMM_WORLD);
	printf("Codigo se ejecuto en %f segundos\n",end_time - start_time);
    free(Resultados);
    MPI_Finalize();
    return 0;
}
