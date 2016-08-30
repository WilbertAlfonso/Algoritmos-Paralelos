#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void llenarM(int *matrix,int fila, int columna)
{
    for (int i=0;i<(fila*columna);i++) 
    {
		*(matrix + i)=1 + (int)(10*random()/(RAND_MAX+1.0));
    }

}
int mostrarM(int *matriz, int fila, int columna)
{
    for (int x = 0; x < fila; x++) 
    {
		for (int y = 0; y < columna; y++) 
		{
	    	printf("%7d ",matriz[x*columna+y]);
		}
		printf("\n");
    }
    return 0;
}
int main(int argc, char **argv)
{
    int my_rank, comm_sz, fila, columna;
    int *matriz1=NULL;
    int *matriz2=NULL;
    int *matriz3=NULL;
    int *tmp;
    MPI_Status status;
    fila=1000;
    columna=1000;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if (my_rank == 0) 
    {
    	int contador=0;
		matriz1 = malloc(fila*columna*sizeof(int));
		matriz2 = malloc(fila*columna*sizeof(int));
		matriz3 = malloc(fila*columna*sizeof(int));
		llenarM(matriz1, fila, columna);
		llenarM(matriz2, fila, columna);
		contador = 0;
		MPI_Bcast(matriz1,fila*columna,MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(matriz2,fila*columna,MPI_INT, 0, MPI_COMM_WORLD);
		tmp=malloc((columna+1)*sizeof(int));
		while (contador<fila) 
		{
		    MPI_Recv(tmp,columna + 1,MPI_INT,MPI_ANY_SOURCE,1, MPI_COMM_WORLD, &status);
		    printf("fila %d de %d\n",tmp[0], status.MPI_SOURCE);
		    contador++;
		    for (int i = 0; i<columna; i++)
		    {
				matriz3[tmp[0]*columna+i]=tmp[i+1];
		    }
		}
	    mostrarM(matriz3, fila, columna);
		if (matriz1!=NULL) free(matriz1);
		if (matriz2!=NULL) free(matriz2);
		if (matriz3!=NULL) free(matriz3);
    }
    else 
    {
		matriz1 = malloc(fila*columna*sizeof(int));
		matriz2 = malloc(fila*columna*sizeof(int));
		int proceso,inicio,fin,salto;
		MPI_Bcast(matriz1,fila*columna,MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(matriz2,fila*columna,MPI_INT, 0, MPI_COMM_WORLD);
		proceso = comm_sz-1;
		salto = (int)(fila/proceso);
		inicio = salto*(my_rank-1);
		fin = inicio+salto-1;
		if (my_rank == comm_sz-1) 
		{
		    fin =fin+(fila%proceso);
		}
		tmp = malloc((columna + 1) * sizeof(int));
		for (int i = inicio; i <= fin; i++) 
		{
		    for (int j = 0; j<columna; j++) 
		    {
				int suma = 0;
				for (int k = 0; k < columna; k++) 
				{
				    suma += matriz1[i*columna+k]*matriz2[k*columna+j];
				}
				tmp[j+1] = suma;
		    }
		    tmp[0] = i;
		    MPI_Send(tmp,columna + 1,MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
	    if (matriz1!=NULL) free(matriz1);
	    if (matriz2!=NULL) free(matriz2);
	    if (matriz3!=NULL) free(matriz3);
    }
    MPI_Finalize();
    return 0;
}
