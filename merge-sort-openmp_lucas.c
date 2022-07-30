/* C program for Merge Sort */
#include <stdio.h>
#include <stdlib.h>
//#include<omp.h>
#include<time.h>

#define SIZE 20000
#define MAX_NUM 1000

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	// Define tamanhos dos vetores que serão combinados
	int n1 = m - l + 1;
	int n2 = r - m;

	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	//Pega os subarrays a partir dos indices e do array principal
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

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
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
	if (l < r) {
		// Same as (l+r)/2, but avoids overflow for
		// large l and h
		int m = l + (r - l) / 2;

		// Sort first and second halves

		/* PARALELIZAR ESSE TRECHO RECURSIVO*/
		mergeSort(arr, l, m);
		/* PARALELIZAR ESSE TRECHO RECURSIVO */
		mergeSort(arr, m + 1, r);

		/* PARALELIZAR ESSE TRECHO RECURSIVO */
		merge(arr, l, m, r);
	}
}

/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int A[], int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", A[i]);
	printf("\n");
}

/* Driver code */
int main()
{
	struct timeval begin, end;
	srand(time(0));

	int arr[SIZE];
	
	for(int i = 0; i<SIZE; i++){
		arr[i] = rand()%MAX_NUM;
	}

	//printf("Given array is \n");
	//printArray(arr, SIZE);

	/*PARALELIZAR ESSE TRECHO*/
	// Start measuring time
    gettimeofday(&begin, 0);
	mergeSort(arr, 0, SIZE - 1);
	gettimeofday(&end, 0);

	//printf("\n\nSorted array is \n");
	//printArray(arr, SIZE);

	long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
	//double elapsed = (seconds + microseconds*1e-6); //Seconds
	//printf("\n\nTotal execution time of mergeSort() function: %f s\n", elapsed);
    double elapsed = (seconds + microseconds*1e-6) * 1000; //Milliseconds
	printf("\n\nTotal execution time of mergeSort() function: %f ms\n", elapsed);
	
	return 0;
}