#pragma once

#include "carta.h"
#include "card_node.h"
#include "pilha.h"
#include "player.h"
#include "game_state.h"


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


// Fun��es do jogo
void initialize_game(int num_players);
void deal_initial_hands(int num_players, int hand_size);
void aplicar_efeito_carta(PlayerNode* jogador_da_vez, Carta* carta);

// Init
void initialize_baralho(Pilha* baralho, int num_cartas);
void deal_initial_hands(int num_players, int hand_size);
CartaNode* create_mao();
PlayerNode* create_player(CartaNode* mao);
void create_player_list(); // coloca todos os jogadores na lista


// Turnos -> Prioridade
void game_loop();
void next_player();
void start_turn_checks(); 
void end_turn_checks();
void verificar_vitoria();


// Player & Relacionados


// PlayerNode, Baralho
void puxar_baralho(PlayerNode** jogador_da_vez, Pilha** baralho); // Chamada para adicionar uma carta em um CartaNode
void puxar_2_cartas(PlayerNode** jogador_da_vez, Pilha** baralho); // Chamada pra adicionar duas cartas
void puxar_4_cartas(PlayerNode** jogador_da_vez, Pilha** baralho); // Chamada pra adicionar quatro cartas

// PlayerNode, Pilha
void jogada(PlayerNode* player, Carta* carta);
int validar_jogada(Carta* carta, Pilha* pilha);
Carta* jogar_pilha(PlayerNode* player, int carta_selecionada);



