#pragma once

#include "stdlib.h"
#include "player.h"
#include "card.h"
#include "card_node.h"
#include "pilha.h"


// Enums
typedef enum {
	ANTI_CLOCKWISE,
	CLOCKWISE
} Direction;


// Structs
typedef struct PlayerNode {
    CardNode* mao;
    struct PlayerNode *next;
    struct PlayerNode *prev;
} PlayerNode;

Card* remove_player_first(PlayerNode **player) {
	if (!player || !*player) return NULL;
	CardNode* aux = *player;
	Card* card = aux->card;
	*player = aux->next;
	free(aux);
	return card;
}


// Init
void init_player_list(); // coloca todos os jogadores na lista

void definir_pilha();
void iniciar_jogo();
void distribuir_cartas(PlayerNode** player, Pilha** baralho, int num_cartas_distribuidas);


// Turnos -> Prioridade
void game_loop();
void next_player();
void start_turn_checks(); 
void end_turn_checks();

// Player & Relacionados
Card* carta_selecionada(CardNode** mao, Card* carta);

Card jogar_carta(CardNode** mao, Pilha** pilha, Card* carta);
CardNode exibir_mao_player(CardNode **mao);
void ordenar_mao(CardNode **mao);


// CardNode, Baralho
void add_card(CardNode** mao, Pilha** baralho); // Chamada para adicionar uma carta em um CardNode
void add_2_cards(CardNode** mao, Pilha** baralho); // Chamada pra adicionar duas cartas
void add_4_cards(CardNode** mao, Pilha** baralho); // Chamada pra adicionar quatro cartas

// CardNode, Pilha
CardNode jogada(CardNode** mao, Pilha* pilha, Card* carta);
Card* validar_jogada(CardNode **mao, Card* carta, Pilha* pilha);
Card* validar_topo(Card* carta, Pilha** pilha);


// Baralho, Pilha
void iniciar_pilha();


// Pilha, Baralho
void passagem_pilha_baralho();

// Player Functions
Player jogador_ganhador(Player atual);

