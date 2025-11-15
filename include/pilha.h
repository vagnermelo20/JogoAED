#pragma once
#include "card_node.h"
#include "card.h"


typedef struct Pilha {
	Card *carta;
	struct Pilha *next;
} Pilha;



// FisherYates Shuffle
void swap(int* a, int* b);
void printArray(int arr[], int n);
void randomize(int arr[], int n);

int count(Pilha* pilha);
void shuffle_deck(Pilha** baralho);
void refill(Pilha** pilha, Pilha** baralho);
void initialize_baralho(Pilha** baralho);
void unload_all_pilha(Pilha** head);