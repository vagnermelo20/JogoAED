#pragma once
#include "carta.h"


typedef struct Pilha {
	Carta *carta;
	struct Pilha *next;
} Pilha;


// Generalized Functions
int count(Pilha* pilha);
Carta* remover_topo(Pilha** pilha);
void unload_all_pilha(Pilha** baralho); // Dá free em todo o baralho

// Baralho, Pilha
void initialize_pilha(Pilha* pilha, Pilha* baralho);


// Pilha, Baralho
void embaralhar(Pilha* pilha, Pilha* baralho);
void refill(Pilha** pilha, Pilha** baralho); // Quando o baralho esvazia, a pilha o reenche


// Pilha
Carta* check_top(Pilha* pilha);


// Baralho
void initialize_baralho(Pilha* baralho, int num_cartas); //Inicializa o baralho
// FisherYates Shuffle
//  swap(int* a, int* b);
// void printArray(int arr[], int n);
// void randomize(int arr[], int n);