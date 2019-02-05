#include <mpi.h>
#include <stdio.h>
#define RANK_MESTRE 0
#define TAG_OPERACOES 50

int main() {
	
	// Initialize the MPI environment. The two arguments to MPI Init are not
	// currently used by MPI implementations, but are there in case future
	// implementations might need the arguments.
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Get the rank of the process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Get the name of the processor
	int name_len;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Get_processor_name(processor_name, &name_len);
	MPI_Status status;


	int a [4][4] = {{1,2,3,4}, {4,3,2,1}, {4,5,6,7}, {2,4,6,8}};
	int b [4][4] = {{6,7,8,9}, {9,8,7,6}, {1,2,4,8}, {1,3,5,7}};
	int c [4][4];

	int sum=0,i,j,k, tam=4;

	// dividindo as tarefas nos vários processos
	for(i = rank; i < tam; i = i+size) 
	{
		for(j = 0; j < tam; j++)
		{
			sum=0;
			for(k = 0; k < tam; k++)
			{
				sum = sum + a[i][k] * b[k][j];
			}
			c[i][j] = sum;
		}
	}
	
	
	if(rank != RANK_MESTRE)
	{
		for(i = rank; i < tam; i = i+size)
			MPI_Send(&c[i][0], tam, MPI_INT, 0, 10+i, MPI_COMM_WORLD);
	
	}
	else
	{
		for(j = 1; j < size; j++){
			for(i = j; i < tam; i = i+size){
				MPI_Recv(&c[i][0], tam, MPI_INT, j, 10+i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0){
		for(i = 0; i < tam; i++){
			for(j = 0; j < tam; j++){
				printf("%d\t",c[i][j]); 
			}
			printf("\n");
		}
	}

	// Finaliza
	if(rank == 0)
		MPI_Finalize();
	return 0;

	// Finalize the MPI environment. No more MPI calls can be made after this
	// MPI_Finalize();
}
