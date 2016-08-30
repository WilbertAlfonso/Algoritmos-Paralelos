#include <mpi.h>
#include <stdio.h>
int main(int argc,char **argv)
{
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	double limit=20;
	if(rank==1)
	{
		double data;
		while(data<limit-1)
		{
			MPI_Recv(&data,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			data=data+1;
			MPI_Send(&data,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
			printf("Proceso %d = %g \n",rank,data);
		}
	}
	if(rank==0)
	{
		double data = 0;
		while(data<limit)
		{
			MPI_Send(&data,1,MPI_DOUBLE,1,0,MPI_COMM_WORLD);
			MPI_Recv(&data,1,MPI_DOUBLE,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			data=data+1;
			printf("Proceso %d = %g \n",rank,data);
		}
	}
	MPI_Finalize();
	return 0;
}