#pragma once
#include "carta.h"


typedef struct CartaNode {
	Carta* carta;
	struct CartaNode* next;
} CartaNode; // Lista de cartas, representa a mao de jogador


int contar_mao(CartaNode* mao); //Contagem de cartas
CartaNode* criar_mao();
void ordenar_mao(CartaNode** mao); 
int comparar_cartas(Carta* carta1, Carta* carta2);
