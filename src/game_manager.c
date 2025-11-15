#pragma once
#include "game_manager.h"
#include "pilha.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



GameState game = {
	.jogador_da_vez = NULL,
	.lista_jogadores = NULL,
	.baralho = NULL,
	.pilha = NULL,
	.direcao = 0,
	.jogador_bloqueado = 0,
	.corAtual = 0,
	.comprar_cartas = 0,
	.jogoTerminado = 0,
	.vencedor = NULL
};

// Vars
static Direction direcao = 0;
static const num_players = 4;
static const num_cartas = 28;




#pragma region INICIANDO_JOGADORES
// Variables Init
int comprar_cartas = 0;
int jogador_bloqueado = 0;

// Carta Node Init
static CartaNode* mao1 = NULL;
static CartaNode* mao2 = NULL;
static CartaNode* mao3 = NULL;
static CartaNode* mao4 = NULL;




void deal_initial_hands(int num_players, int hand_size) {
	// Distribuir cartas iniciais para cada jogador
	for (int i = 0; i < num_players; i++) {
		for (int j = 0; j < hand_size; j++) {
			puxar_baralho(game.jogador_da_vez, game.baralho);
		}
		game.jogador_da_vez = game.jogador_da_vez->next;
	}
}


PlayerNode* create_player(CartaNode* mao) {
    PlayerNode* newPlayer = malloc(sizeof(PlayerNode));
    newPlayer->mao = mao;
    newPlayer->next = NULL;
    newPlayer->prev = NULL;
    
    return newPlayer;
}

void create_player_list() {
	PlayerNode* p1 = create_player(mao1);
	PlayerNode* p2 = create_player(mao2); 
	PlayerNode* p3 = create_player(mao3); 
	PlayerNode* p4 = create_player(mao4);

	p1->next = p2;
	p2->next = p3;
	p3->next = p4;
	p4->next = p1;

	p1->prev = p4;
	p4->prev = p3;
	p3->prev = p2;
	p2->prev = p1;

	game.jogador_da_vez = p1;
	game.lista_jogadores = p1;
}


// Cria um baralho completo de UNO
void criar_baralho(Pilha* baralho) {
	int index = 0;

	// Para cada cor (exceto curinga)
	for (int cor = VERMELHO; cor <= AZUL; cor++) {
		// Um zero de cada cor
		Carta carta;
		carta.cor = cor;
		carta.valor = ZERO;
		adicionar_carta(game.baralho, carta);

		// Dois de cada carta de 1-9 e especiais
		for (int copia = 0; copia < 2; copia++) {
			for (int valor = UM; valor <= BLOQUEAR; valor++) {
				carta.cor = cor;
				adicionar_carta(game.baralho, carta);
			}
		}
	}

	// 4 curingas simples
	for (int i = 0; i < 4; i++) {
		Carta carta;
		carta.cor = INCOLOR;
		carta.valor = BLOQUEAR; // Valor não importa para curinga simples, mas não pode ser MAIS_QUATRO
		adicionar_carta(baralho, carta);
	}

	// 4 curingas +4
	for (int i = 0; i < 4; i++) {
		Carta carta;
		carta.cor = INCOLOR;
		carta.valor = MAIS_4;
		adicionar_carta(baralho, carta);
	}
}


// Inicializa o estado do jogo
void inicializarJogo(int numJogadores) {


	criar_baralho(game.baralho);
	embaralhar(game.pilha, game.baralho);

	// Primeira carta na pilha
	Carta* primeiraCarta = remover_topo(&(game.baralho));
	while (primeiraCarta != NULL && primeiraCarta->valor >= MAIS_2) {
		// Não começar com cartas especiais
		initialize_pilha(game.pilha, game.baralho);
		embaralhar(game.pilha, game.baralho);
		primeiraCarta = remover_topo(game.baralho);
	}

	if (primeiraCarta != NULL) {
		jogada(game.pilha, primeiraCarta);
		game.corAtual = primeiraCarta->cor;
		free(primeiraCarta);
	}

}


void initialize_game(int num_players) {
	initialize_baralho(game.baralho, num_cartas);
	create_player_list();
	embaralhar(game.pilha, game.baralho);
	deal_initial_hands(num_players, 7);
	initialize_pilha(game.pilha, game.baralho);
	// start_game();
}





void next_player() {
	if (direcao == 0) {
		game.jogador_da_vez = game.jogador_da_vez->next;
	} else {
		game.jogador_da_vez = game.jogador_da_vez->prev;
	}
}

#pragma endregion INICIANDO_JOGADORES


// Turnos
#pragma region TURNOS 
void game_loop() {
	while (1) {
		if (IsKeyPressed(KEY_ENTER)) {
			start_turn_checks();
		}
		next_player();
		end_turn_checks();
	}
}


void start_turn_checks() {
	// printa a m�o de quem vai jogar
	// no formato [numero, cor] e.g. [0, amarelo]
	PlayerNode* aux = game.jogador_da_vez;
	while (aux->mao->carta != NULL) {
		TraceLog(LOG_INFO, "[", aux->mao->carta->valor,",", aux->mao->carta->cor, "]");
		Carta* remove_player_first(jogador_da_vez);
	}
}


void end_turn_checks() {


	//Debbuging 
	TraceLog(LOG_INFO, "TESTE");

	// checar se a m�o do cara que jogou est� vazia.
	// Se estiver, encerra o jogo. D� um return, exit, printf cabou ou qualquer coisa.
	if (game.jogador_da_vez->mao->carta == 0) {
		TraceLog(LOG_INFO, "jogador ganhou -> ", game.jogador_da_vez);
	}
	// printe a m�o de todos os jogadores.
	PlayerNode* aux = game.lista_jogadores;
	do {
		while (aux->mao->carta != NULL) {
			TraceLog(LOG_INFO, "|", game.lista_jogadores->mao->carta, "|");
		}
		aux = aux->next;
	} while (aux != game.lista_jogadores);
	// flags setadas baseadas em valores, acima de 9 -> passa efeitos para o pr�ximo.
}

#pragma endregion TURNOS 

// Player & Relacionados




// Update
void atualizar_partida() {}

// Draw
void desenhar_partida() {}



// CartaNode, Baralho
#pragma region PlayerNodeBaralho

void puxar_baralho(PlayerNode** jogador_da_vez, Pilha** baralho) { // Jogador puxa uma carta do baralho
	if (!*jogador_da_vez || !jogador_da_vez || !*baralho || !baralho || !((*baralho)->carta)) return;
	if (count(baralho) == 0) {
		refill(game.pilha, *baralho);
	}

	Pilha* aux = *baralho;
	CartaNode* novo = malloc(sizeof(CartaNode));
	novo->next = (*jogador_da_vez)->mao;
	novo->carta = (*baralho)->carta;
	(*jogador_da_vez)->mao = novo;
	*baralho = (*baralho)->next;

	free(aux);
	return;
}

// Fun��o ser� usada depois quando um jogador receber um +2
void puxar_2_cartas(PlayerNode** jogador_da_vez, Pilha** baralho) {
	if (!*jogador_da_vez || !jogador_da_vez || !*baralho || !baralho || !((*baralho)->carta)) return;;
	for (int i = 0; i < 2; i++) puxar_baralho(jogador_da_vez, &baralho);
	return;
}

// Fun��o ser� usada depois quando um jogador receber um +4
void puxar_4_cartas(PlayerNode** jogador_da_vez, Pilha** baralho) {
	if (!*jogador_da_vez || !jogador_da_vez || !*baralho || !baralho || !((*baralho)->carta)) return;
	for (int i = 0; i < 4; i++) puxar_baralho(*jogador_da_vez, &baralho);
	return;
}

#pragma endregion


// CartaNode, Pilha
#pragma region CartaNodePilha

// Aqui se valida a carta sendo jogada com a carta no topo da pilha, retornando 1 caso seja validada e 0 caso n�o
int validar_jogada(Carta* carta, Pilha* pilha) {
	if (!(pilha) || !carta) return 0;
	if (check_top(pilha)->cor == carta->cor || check_top(pilha)->valor == carta->valor) return 1;
	return 0;
}

void jogada(PlayerNode* player, Carta* carta) {
	if (!player || !carta) return;
	CartaNode* aux = player->mao;
	CartaNode* temp;
	while (!validar_jogada(carta, game.pilha)) {
		if (aux == NULL) {
			free(aux);
			free(temp);
			return;
		}
		Pilha* nova = malloc(sizeof(Pilha));
		nova->carta = aux->carta;
		nova->next = game.pilha;
		game.pilha = nova;
		temp = aux->next;
		free(aux);
		free(temp);
	}
	temp->next = temp->next->next;
	
}

Carta* jogar_pilha(PlayerNode* player, int carta_selecionada) {
	if (!player || !carta_selecionada || count(player->mao) < carta_selecionada) return (Carta*) { 0 };
	Pilha* nova = malloc(sizeof(Pilha));
	CartaNode* aux = player->mao;
	for (int i = 0; i < carta_selecionada - 1; i++) {
		aux = aux->next;
	}
	CartaNode* temp = aux->next;
	nova->carta = temp->carta;
	nova->next = game.pilha;
	game.pilha = nova;
	free(temp);
	aux->next = aux->next->next;
	free(aux);
	return nova->carta;
}

#pragma endregion CartaNodePilha


// Baralho, Pilha
#pragma region BaralhoPilha


#pragma endregion BaralhoPilha


// Pilha, Baralho 
#pragma region PilhaBaralho


#pragma endregion PilhaBaralho


Carta* remove_player_first(PlayerNode** player) { // Mock Function
	if (!player || !*player) return NULL;
	CartaNode* aux = (*player)->mao;
	Carta* card = aux->carta;
	(*player)->mao = aux->next;
	free(aux);
	return card;
}



// Aplica efeitos especiais das cartas
void aplicar_efeito_carta(PlayerNode player, Carta* carta) {
	switch (carta->valor) {
		case MAIS_2:
			// +2 s� aplica efeito se N�O for INCOLOR (cartas coridas)
			if (carta->cor != INCOLOR) {
				comprar_cartas += 2;
			}
			break;

		case MAIS_4:
			// +4 sempre aplica efeito (s� existe em INCOLORs)
				comprar_cartas += 4;
			break;

		case REVERSO:
			direcao = !direcao;
			break;

		case BLOQUEAR:
			jogador_bloqueado = 1;
			break;

		default:
			break;
	}
}

// Libera mem�ria do jogo
void liberarJogo() {

	// Liberar jogadores
	if (game.jogador_da_vez != NULL) {
		PlayerNode* inicio = game.jogador_da_vez;
		PlayerNode* temp = inicio;
		do {
			PlayerNode* proximo = temp->next;

			CartaNode* mao = temp->mao;
			while (mao != NULL) {
				CartaNode* proxCarta = mao->next;
				mao = proxCarta;
			}

			free(temp);
			temp = proximo;
		} while (temp != inicio);
	}

	// Liberar baralho
	while (game.baralho != NULL) {
		Pilha* temp = game.baralho;
		game.baralho = temp->next;
		free(temp);
	}

	free(game.baralho);

	// Liberar pilha de jogo
	while (game.pilha != NULL) {
		Pilha* temp = game.pilha;
		game.pilha = temp->next;
		free(temp);
	}
	free(game.pilha);

	//Free Assets
}