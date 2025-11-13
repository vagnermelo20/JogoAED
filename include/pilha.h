#pragma once
#include "card_node.h"
#include "card.h"


typedef struct Pilha {
	Card *carta_pilha;
	struct Pilha *next;
} Pilha;

int count(Pilha* pilha);
void iniciar_baralho(Pilha** pilha);
void add_pilha(Pilha **head, Card* card);
void unload_all_pilha(Pilha** head);