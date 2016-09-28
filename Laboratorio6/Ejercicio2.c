#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING=100;

int main (void)
{
	//char greeting[MAX_STRING];
	int comm_sz;
	int my_rank;
	int per;
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

	if(my_rank+1==comm_sz)
	{
		MPI_Recv(&per,1,MPI_INT,my_rank-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		printf("Greeting from process %d from %d!\n", my_rank,comm_sz);	
	}
	else
	{
		if(my_rank!=0)
		{
			MPI_Recv(&per,1,MPI_INT,my_rank-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			printf("Greeting from process %d from %d!\n", my_rank,comm_sz);
			MPI_Send(&per,1,MPI_INT,my_rank+1,0,MPI_COMM_WORLD);
		}
		else
		{
			printf("Greeting from process %d from %d!\n", my_rank,comm_sz);
			per=1;
			MPI_Send(&per,1,MPI_INT,1,0,MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
	return 0;
}