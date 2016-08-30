#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
void M_Broad(float* mensaje,int tam)
{
    int my_rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int mask=size-1;
    int d = (int)(log(size)/log(2));
    for(int k=d-1;k>=0;k--)
    {
        mask=mask ^ (int)(pow(2,k));
        if((my_rank & mask) == 0)
        {
            int punto=my_rank ^ (int)(pow(2,k));
            if((my_rank & (int)(pow(2,k))) == 0)
                MPI_Send(mensaje,tam,MPI_FLOAT,punto,0,MPI_COMM_WORLD);
            else
                MPI_Recv(mensaje,tam,MPI_FLOAT,punto,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
        }
    }
}
/*void M_Reduce(float* mensaje,float* salida,int tam)
{
    int my_rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int mask=size-1;
    int d = (int)(log(size)/log(2));
    for(int k=0;k<d;k++)
    {
        mask=mask ^ (int)(pow(2,k));
        if((my_rank & mask) == 0)
        {
            int punto=my_rank ^ (int)(pow(2,k));
            if((my_rank & (int)(pow(2,k))) == 0)
            {
                MPI_Recv(salida,tam,MPI_FLOAT,punto,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            }
            else
            {
                MPI_Send(mensaje,tam,MPI_FLOAT,punto,0,MPI_COMM_WORLD); 
            }
        }
    }
}*/
int main(int argc, char** argv)
{
    double start_time, end_time;
    int size, rank;
    float mensaje;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();
    if(rank != 0)
    {
        M_Broad(&mensaje,1);
        //MPI_Bcast(&mensaje,1,MPI_FLOAT, 0, MPI_COMM_WORLD);
        printf("mensaje = %g\n", mensaje);
    }
    else
    {
        mensaje=93;
        //MPI_Bcast(&mensaje,1,MPI_FLOAT, 0, MPI_COMM_WORLD);
        M_Broad(&mensaje,1);
    }
    MPI_Barrier(MPI_COMM_WORLD); 
    end_time= MPI_Wtime();
    printf("Codigo se ejecuto en %f segundos\n",end_time - start_time);

    MPI_Finalize();

    return (EXIT_SUCCESS);
}