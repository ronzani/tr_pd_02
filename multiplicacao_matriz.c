#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


//Funcao para imprimir uma matriz
void imprime_matriz(int **m, int tam){
	for(int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j++){
			printf("%d\t", m[i][j]); 
		}
		printf("\n");
	}
}


int main(int argc , char *argv[]){
	
	int tam = atoi(argv[1]);
	int size,rank,sum=0,i,j,k;
	int **a, **b, **c;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// criando um array de ponteiros
	a =(int **)malloc(tam * sizeof(int*));
	b =(int **)malloc(tam * sizeof(int*));
	c=(int **)malloc(tam * sizeof(int*));

	// alocando memória para as matrizes
	for(i = 0; i < tam; i++){
		a[i]=(int *)malloc(tam * sizeof(int));
		c[i]=(int *)malloc(tam * sizeof(int));
		b[i]=(int *)malloc(tam * sizeof(int));
	}
	
	// inicializando as matrizes
	for(i = 0; i < tam; i++){
		for(j = 0; j < tam; j++){
			a[i][j] = 1;
			b[i][j] = 2;
			c[i][j] = 0;
		}
	}
		
	//Imprime os parametros e as matrizes
	if (rank == 0){
		printf("\n");
		printf("Paremetros:");
		printf("\n");
		printf("Tamanho da Matriz: %d", tam);
		printf("\n");
		printf("Numero de Processos: %d", size);

		printf("\n\n\n");
		printf("Matriz A");
		printf("\n\n");

		imprime_matriz(a, tam);

		printf("\n\n");
		printf("Matriz B");
		printf("\n\n");

		imprime_matriz(b, tam);

		printf("\n\n");
	}


	// Divide as tarefas entre os processos
	for(i = rank; i < tam; i = i+size){
		for(j = 0; j < tam; j++){
			sum=0;
			for(k = 0; k < tam; k++){
				sum = sum + a[i][k] * b[k][j];
			}
			c[i][j] = sum;
		}
	}

	// Imprime a Matriz C calculada por cada processo
	for (i=0; i <= size; i++){
		MPI_Barrier(MPI_COMM_WORLD);
		if (rank == i){
			printf("\n\n");
			printf(">>>>>>>> ------- <<<<<<<<<<");
			printf("\n");
			printf("Processo rank %d", rank);
			printf("\n");
			printf("Matriz C");
			printf("\n\n");
			imprime_matriz(c, tam);
		}
	}
 
 	// Se o rank for diferente de um, ele envia o resultado para o rank principal
	if(rank != 0){
		for(i = rank; i < tam; i = i+size)
			MPI_Send(&c[i][0], tam, MPI_INT, 0, 10+i, MPI_COMM_WORLD);
	}

	// Sendo o rank principal, ele vai receber todas as partes calculadas de cada um dos outros processos
	if(rank == 0){
		for(j = 1; j < size; j++){
			for(i = j; i < tam; i = i+size){
				MPI_Recv(&c[i][0], tam, MPI_INT, j, 10+i, MPI_COMM_WORLD, &status);
			}
		}
	}

	// Espera até ter o resultado completo
	MPI_Barrier(MPI_COMM_WORLD);
	
	//Imprime a matriz resultdo
	if(rank == 0){
		printf("\n\n");
		printf(">>>>>>>> ------- <<<<<<<<<<");
		printf("\n");
		printf("Matriz Resultado");
		printf("\n");
		
		imprime_matriz(c, tam);
	}

	MPI_Finalize();
	
	return 0;
}
