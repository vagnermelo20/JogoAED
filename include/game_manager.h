#pragma once

#include "carta.h"
#include "card_node.h"
#include "pilha.h"


// Structs
typedef struct PlayerNode {
    char numero;
    CartaNode* mao;
    struct PlayerNode *next;
    struct PlayerNode *prev;
} PlayerNode;


// Enums
typedef enum Direction {
	ANTI_CLOCKWISE,
	CLOCKWISE
} Direction;


 typedef struct GameState {
	PlayerNode* jogador_da_vez;
	PlayerNode* lista_jogadores;
	Pilha* baralho;
	Pilha* pilha;
	Direction direcao;
	int jogador_bloqueado;
	Cor corAtual;
	int comprar_cartas;
	int jogoTerminado;
	PlayerNode* vencedor;
} GameState;

 extern GameState game;


void initialize_game(int num_players);
void deal_initial_hands(int num_players, int hand_size);
void aplicar_efeito_carta(PlayerNode* jogador_da_vez, Carta* carta);

CartaNode* criar_mao();
PlayerNode* create_player(CartaNode* mao);
void create_player_list();
void next_player();
void verificar_vitoria();
void puxar_baralho(PlayerNode** jogador_da_vez, Pilha** baralho);
void puxar_2_cartas(PlayerNode** jogador_da_vez, Pilha** baralho);
void puxar_4_cartas(PlayerNode** jogador_da_vez, Pilha** baralho);
int validar_jogada(Carta* carta, Pilha* pilha);
Carta* jogar_pilha(PlayerNode* player, int carta_selecionada);
void liberarJogo();


