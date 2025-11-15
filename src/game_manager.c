#pragma once
#include "game_manager.h"
#include "pilha.h"
#include "raylib.h"


// Vars

static Direction direcao = 0;
static const cartas = 28;

// 
Pilha* pilha = NULL;
Pilha* baralho = NULL;

#pragma region INICIANDO_JOGADORES

// Card Node Init
static CardNode* mao1 = NULL;
static CardNode* mao2 = NULL;
static CardNode* mao3 = NULL;
static CardNode* mao4 = NULL;

// Player Turn Init
static PlayerNode* lista_jogadores = NULL;
static PlayerNode* jogador_da_vez = NULL;

// Init

void initialize_baralho(Pilha** baralho) {
	if (*baralho != NULL) {
		return;
	}
	for (baralho = 0; baralho < cartas; baralho++) {
		Pilha* nova = (Pilha*)malloc(sizeof(Pilha));
		if (nova == NULL) return;
		nova->carta->valor = GetRandomValue(0, 9);
		nova->carta->cor = GetRandomValue(1, 4);
		add_pilha(&baralho, nova);
		*baralho = nova;
	}
}

void shuffle_deck(Pilha** baralho){
	
}

void initialize_pilha(Pilha** pilha, Pilha** baralho) {
	if (*pilha != NULL) {
		return;
	}
	(*pilha) = (Pilha*)malloc(sizeof(Pilha));
	(*pilha)->carta = get_top_discard(baralho);
	(*pilha)->next = NULL;
}


void start_game() {
	// Implementar lógica para iniciar o jogo

}


void initialize_game(int num_players) {
	initialize_baralho(baralho);
	create_player_list(num_players);
	deal_initial_hands(num_players, 7);
	shuffle_deck(baralho);
	start_game();
}


void deal_initial_hands(int num_players, int hand_size) {
	// Distribuir cartas iniciais para cada jogador
	PlayerNode* current_player = lista_jogadores;
	for (int i = 0; i < num_players; i++) {
		for (int j = 0; j < hand_size; j++) {
			add_card(current_player->mao, baralho);
		}
		current_player = current_player->next;
	}
}

PlayerNode* create_player(CardNode* playerHand) {
    PlayerNode* newPlayer = malloc(sizeof(PlayerNode));
    newPlayer->mao = playerHand;
    newPlayer->next = NULL;
    newPlayer->prev = NULL;
    
    return newPlayer;
}

void init_player_list() {
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

	jogador_da_vez = p1;
	lista_jogadores = p1;
}

void next_player() {
	if (direcao == 0) {
		jogador_da_vez = jogador_da_vez->next;
	} else {
		jogador_da_vez = jogador_da_vez->prev;
	}
}

#pragma endregion INICIANDO_JOGADORES

//

#pragma region MOCK



#pragma endregion MOCKEND


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
	// printa a mão de quem vai jogar
	// no formato [numero, cor] e.g. [0, amarelo]
	PlayerNode* aux = jogador_da_vez;
	while (aux->mao->card != NULL) {
		TraceLog(LOG_INFO, "[", aux->mao->card->valor,",", aux->mao->card->cor, "]");
		Card* remove_player_first(jogador_da_vez);
	}
}


void end_turn_checks() {


	//Debbuging 
	TraceLog(LOG_INFO, "TESTE");

	// checar se a mão do cara que jogou está vazia.
	// Se estiver, encerra o jogo. Dá um return, exit, printf cabou ou qualquer coisa.
	if (jogador_da_vez->mao->card == 0) {
		TraceLog(LOG_INFO, "jogador ganhou -> ", jogador_da_vez);
	}
	// printe a mão de todos os jogadores.
	PlayerNode* aux = lista_jogadores;
	do {
		while (aux->mao->card != NULL) {
			TraceLog(LOG_INFO, "|", lista_jogadores->mao->card, "|");
		}
		aux = aux->next;
	} while (aux != lista_jogadores);
	// flags setadas baseadas em valores, acima de 9 -> passa efeitos para o próximo.
}  


void definir_pilha() {
	CardNode* novo = malloc(sizeof(CardNode));
	for (int i = 0; i < cartas; i++) {
		novo->card->valor = GetRandomValue(0, 15);
		novo->card->cor = GetRandomValue(0, 4);
		add_card(&pilha, novo->card);
		novo = novo->next;
	}
}



// Player & Relacionados
Card jogar_carta(mao, pilha) {

}

// DeInit
void acabarJogo() {
}

void descarregar_partida() {}

// Update
void atualizar_partida() {}

// Draw
void desenhar_partida() {}


// Aux

void add_card(CardNode** mao, Pilha **baralho) {
	if (!*mao || !mao || !*baralho || !baralho) return;
	return;
}

// Função será usada depois quando um jogador receber um +2
void add_2_cards(CardNode** mao, Pilha** baralho) {
	if (!(*mao) || !mao || !(*baralho) || !baralho) return;

	if (count_baralho(baralho) < 2) {
	}

	for (int i = 0; i < 2; i++) add_card(*mao, *baralho);
	return;
}

// Função será usada depois quando um jogador receber um +4
void add_4_cards(CardNode** mao, Pilha** baralho) {
	if (!*mao || !mao || !*baralho || !baralho) return;
	for (int i = 0; i < 4; i++) add_card(*mao, *baralho);
}

Card* remove_first(Pilha** pilha) {
	if (!pilha || !*pilha) return NULL;
	CardNode* aux = *pilha;
	Card* card = aux->card;
	*pilha = aux->next;
	free(aux);
	return card;
}

Card* remove_player_first(PlayerNode** player) {
	if (!player || !*player) return NULL;
	CardNode* aux = *player;
	Card* card = aux->card;
	*player = aux->next;
	free(aux);
	return card;
}