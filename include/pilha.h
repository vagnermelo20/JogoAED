#pragma once
#include "carta.h"


typedef struct Pilha {
    Carta *carta;
    struct Pilha *next;
} Pilha;


// Generalized Functions
int count(Pilha* pilha);
Carta* remover_topo(Pilha** pilha);
void adicionar_carta(Pilha** pilha, Carta carta);
void unload_all_pilha(Pilha** baralho);

// Baralho, Pilha
void initialize_pilha(Pilha** pilha, Pilha** baralho); 

// Pilha, Baralho
void embaralhar(Pilha** pilha, Pilha** baralho); 
void refill(Pilha** pilha, Pilha** baralho);

// Pilha
Carta* check_top(Pilha* pilha);

// Baralho
void initialize_baralho(Pilha** baralho, int num_cartas); 
