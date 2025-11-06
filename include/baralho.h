#pragma once
#include "card.h"


typedef struct Baralho {
	Card *carta_baralho;
	Card *next;
} Baralho;

void iniciarBaralho(Baralho** baralho);