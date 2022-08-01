/* C program for Merge Sort */
#include <stdio.h>
#include <stdlib.h>
#include<omp.h>
#include<time.h>

#define SIZE 1000000
#define MAX_NUM 100

int thread_num = 0;
int max_threads = 0;
int num_threads = 0;
int num_procs = 0;
double elapsed, begin, end = 0;
long microseconds, seconds;

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
	// #if _OPENMP
	// 		printf("\nThread no %d chamou merge()\n", omp_get_thread_num());
	// #endif

	int i, j, k;
	// Define tamanhos dos vetores que serão combinados
	int n1 = m - l + 1;
	int n2 = r - m;

	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	//Pega os subarrays a partir dos indices e do array principal
	for (i = 0; i < n1; i++){
		L[i] = arr[l + i];
	}

	for (j = 0; j < n2; j++){
		R[j] = arr[m + 1 + j];
	}

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray

	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
	//}
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
	//Mostra qual thread está executando esse trecho no momento
	// #if _OPENMP
	// 		printf("\nThread no %d chamou mergeSort()\n", omp_get_thread_num());
	// #endif

	if (l < r) {
		// O mesmo que (l+r)/2, mas evita overflow para l e r muito grandes
		int m = l + (r - l) / 2;

		// Ordena primeira e segunda metades

		//Inicializa tarefa (nova thread filha com metade do array)
		#pragma omp task
		mergeSort(arr, l, m);
		
		//Inicializa outra tarefa (nova thread filha com outra metade do array)
		#pragma omp task
		mergeSort(arr, m + 1, r);

		//Aguarda todas as threads filhas terminarem para seguir com o merge (etapa sequencial)
		#pragma omp taskwait
		merge(arr, l, m, r);
	}
}

/* Função para printar um array */
void printArray(int A[], int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", A[i]);
	printf("\n");
}

/* PRINCIPAL */
int main(int argc, char** argv)
{
	//Mostra o máximo de threads e processadores (cores) que a máquina possui.
	//Castor possui 12 núcleos, portanto pode comportar até 12 threads rodando simultaneaemente.
	// #if _OPENMP
	// {
	// 	max_threads = omp_get_max_threads();
	// 	num_procs = omp_get_num_procs();
	// }
	// #endif

	double begin, end;	
	struct timeval b, e;

	//Gera semente aletória para incializar o array numérico
	srand(time(0));

	int arr[SIZE];
	
	//Gera array aleatoriamente
	for(int i = 0; i<SIZE; i++){
		arr[i] = rand()%MAX_NUM;
	}

	// Printa o array inicial
	// printf("Array inicial:\n");
	// printArray(arr, SIZE);

	//Pega o tempo inicial
	#if _OPENMP
	{
		begin = omp_get_wtime();
	}
	#else
	{
		gettimeofday(&b, 0);
	}
	#endif

	//Inicializa região paralela
	#pragma omp parallel
	{
		//Inicializa algoritmo com única thread (principal)
		#pragma omp single nowait
		{
			mergeSort(arr, 0, SIZE - 1);
		}
	}

	//Marca o tempo final
	#if _OPENMP
	{
		end = omp_get_wtime();
	}
	#else
	{
		gettimeofday(&e, 0);
	}
	#endif

	// Printa o array ordenado
	// printf("\n\nArray ordenado:\n");
	// printArray(arr, SIZE);

	//Calcula o tempo total de execução a partir da primeira chamada do algoritmo mergeSort()
	#if _OPENMP
	{
		elapsed = end - begin;
	}
	#else
	{
		long seconds = e.tv_sec - b.tv_sec;
		microseconds = e.tv_usec - b.tv_usec;
		elapsed = (seconds + microseconds*1e-6) * 1000; //Milliseconds
	}
	#endif
	
	printf("\n\nTotal execution time of mergeSort() function: %ld us\n", microseconds);
	printf("\n\nTotal execution time of mergeSort() function: %f ms\n", elapsed);

	return 0;
}