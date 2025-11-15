#include "pilha.h"
#include <time.h>

// FisherYates Shuffle

// A utility function to swap to integers
void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// A utility function to print an array
void printArray(int arr[], int n)
{
	for (int i = 0; i < n; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

// A function to generate a random permutation of arr[]
void randomize(int arr[], int n)
{
	// Use a different seed value so that we don't get same
	// result each time we run this program
	srand(time(NULL));

	// Start from the last element and swap one by one. We don't
	// need to run for the first element that's why i > 0
	for (int i = n - 1; i > 0; i--)
	{
		// Pick a random index from 0 to i
		int j = rand() % (i + 1);

		// Swap arr[i] with the element at random index
		swap(&arr[i], &arr[j]);
	}
}
int count_baralho(Pilha* pilha) {}
void iniciar_baralho(Pilha** pilha) {}
void add_pilha(Pilha** head, Card* card) {}
void unload_all_pilha(Pilha** head) {}