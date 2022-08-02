/* C program for Merge Sort */
#include <stdio.h>
#include <stdlib.h>
#include<omp.h>
#include<time.h>

#define SIZE 10
#define MAX_NUM 10

int thread_num = 0;
int max_threads = 0;
int num_threads = 0;
int num_procs = 0;
double elapsed, begin, end = 0;
long microseconds, seconds = 0;

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

	/* Arrays temporarios */
	int L[n1], R[n2];

	//Pega os subarrays a partir dos indices e do array principal
	for (i = 0; i < n1; i++){
		L[i] = arr[l + i];
	}

	for (j = 0; j < n2; j++){
		R[j] = arr[m + 1 + j];
	}

	/* Junta os arrays de volta em arr[l..r]*/
	i = 0; // Indice inicial do primeiro subarray
	j = 0; // Indice inicial do segundo subarray
	k = l; // Indice inicial do array mesclado

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

	//Copia elementos restantes de L[], se houver
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	//Copia elementos restantes de R[], se houver
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}

//l é o índice do array da esquerda, r o da direita.
void mergeSort(int arr[], int l, int r)
{
	//Mostra qual thread está executando esse trecho no momento
	// #if _OPENMP
	// 	printf("\nThread no %d chamou mergeSort()\n", omp_get_thread_num());
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
		//#pragma omp taskwait
		//ADICIONAR TASK AQUI COM ELEMENTOS CRITICAL?
		//merge(arr, l, m, r);
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

	struct timeval b, e;

	//Gera semente aletória para incializar o array numérico
	srand(time(0));

	//int arr[SIZE];
	int* arr = (int*) malloc(SIZE * sizeof(int));
	
	//Gera array aleatoriamente
	for(int i = 0; i<SIZE; i++){
		arr[i] = rand()%MAX_NUM;
	}

	//Printa o array inicial
	// printf("Array inicial:\n");
	// printArray(arr, SIZE);

	//Marca o tempo inicial
	gettimeofday(&b, NULL);
	
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
	gettimeofday(&e, NULL);

	//Printa o array ordenado
	printf("\n\nArray ordenado:\n");
	printArray(arr, SIZE);

	//Calcula o tempo total de execução a partir da primeira chamada do algoritmo mergeSort()
	seconds = e.tv_sec - b.tv_sec;
	microseconds = e.tv_usec - b.tv_usec;
	elapsed = (seconds + microseconds*1e-6) * 1000; //Milliseconds

	printf("\n\nTotal execution time of mergeSort() function: %f ms\n", elapsed);

	return 0;
}