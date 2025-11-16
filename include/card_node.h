#pragma once
#include "carta.h"


typedef struct CartaNode {
	Carta* carta;
	struct CartaNode* next;
} CartaNode; // Lista de cartas, representa a mao de jogador


int count_mao(CartaNode* mao); //Contagem de cartas
CartaNode* create_mao();
// Carta* carta_selecionada(CartaNode** mao, Carta* carta);
// CartaNode* exibir_mao_player(CartaNode* mao);
// void ordenar_mao(CartaNode **mao);
// void remove_all_cards(CartaNode** mao); // Chamada para remover todas as cartas