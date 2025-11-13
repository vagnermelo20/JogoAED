#pragma once
#include "card.h"


typedef struct CardNode {
	Card* card;
	struct CardNode *next;
} CardNode; // Lista de cartas, representa a mao de jogador

int count(CardNode* mao); //Contagem de cartas
void remove_all_cards(CardNode** head); // Chamada para remover todas as cartas

void ordenar_mao(CardNode **mao);