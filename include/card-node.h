#pragma once

#include "card.h"

typedef struct CardNode {
	Card* card;
	struct CardNode *next;
} CardNode; // Lista de cartas, pode representar um baralho ou mão de jogador

int count(CardNode *cardNode); //Contagem de cartas
void add_card(CardNode **cardNode, Card* card); // Chamada para adicionar uma carta em um CardNode
void remove_card(CardNode** cardNode, Card *card); // Chamada para jogar uma carta | remover uma carta de uma lista
Card* remove_first(CardNode** cardNode); // Chamada para remover a primeira carta de uma lista