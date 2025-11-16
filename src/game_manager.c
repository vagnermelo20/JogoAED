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
static const num_cartas = 108;




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
		game.jogador_da_vez->numero = i;
		for (int j = 0; j < hand_size; j++) {
			puxar_baralho(&game.jogador_da_vez, &game.baralho);
		}
		game.jogador_da_vez = game.jogador_da_vez->next;
	}
}



PlayerNode* create_player(CartaNode* mao) {
    PlayerNode* newPlayer = malloc(sizeof(PlayerNode));
    newPlayer->mao = mao = create_mao();
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

void initialize_game(int num_players) {
	create_player_list();
    // criar_baralho(game.baralho); talvez implementar
	initialize_baralho(&game.baralho, num_cartas);
	embaralhar(&game.baralho, &game.pilha);
	deal_initial_hands(num_players, 7);
	initialize_pilha(&game.pilha, &game.baralho);
	Carta* primeiraCarta = remover_topo(&game.baralho);
	while (primeiraCarta != NULL && primeiraCarta->valor >= MAIS_2) {
		// Não começar com cartas especiais (embaralhar de novo)
		// initialize_pilha(&game.pilha, &game.baralho);
		// embaralhar(&game.pilha, &game.baralho);
		primeiraCarta = remover_topo(&game.baralho);
	}
	if (primeiraCarta != NULL) {
		Pilha* novo_pilha = malloc(sizeof(Pilha));
		if (novo_pilha) {
			novo_pilha->carta = primeiraCarta;
			novo_pilha->next = game.pilha;
			game.pilha = novo_pilha;
		}

		game.corAtual = primeiraCarta->cor;
	}
	// start_game();
	return;
}





void next_player() {
	if (direcao == 0) {
		game.jogador_da_vez = game.jogador_da_vez->next;
	} else {
		game.jogador_da_vez = game.jogador_da_vez->prev;
	}
}

#pragma endregion INICIANDO_JOGADORES


void verificar_vitoria() {

	if (game.lista_jogadores == NULL || game.jogador_da_vez == NULL || game.baralho == NULL || game.jogador_da_vez->mao == NULL) {
		return;
	}

	PlayerNode* temp = game.lista_jogadores;

	do {
		// Usar count_mao (de card_node.c)
		if (count_mao(temp->mao) == 0) {
			game.jogoTerminado = 1;
			game.vencedor = temp; // Atribui o ponteiro PlayerNode*
			TraceLog(LOG_INFO, "JOGO TERMINADO! Vencedor: p%d", game.vencedor->numero);
			return;
		}

		temp = temp->next;
		if (temp == NULL) break;

	} while (temp != game.lista_jogadores);
}

#pragma endregion TURNOS 

// Player & Relacionados




// Update
void atualizar_partida() {}

// Draw
void desenhar_partida() {}



// CartaNode, Baralho
#pragma region PlayerNodeBaralho

void puxar_baralho(PlayerNode** jogador_da_vez, Pilha** baralho) {
	if (!jogador_da_vez || !*jogador_da_vez || !baralho || !*baralho) {
		TraceLog(LOG_WARNING, "Puxar baralho falhou: jogador ou baralho nulo.");
		return;
	}

	// if (count(baralho) == 0) { // 'count' é de pilha.c
	// 	refill(game.pilha, *baralho);
	// }

	// Verificar se o baralho (pilha) está vazio
	if (*baralho == NULL) {
		TraceLog(LOG_INFO, "Baralho vazio, recarregando...");
		// refill(&game.pilha, baralho); // 'refill' é de pilha.c
		if (*baralho == NULL) {
			TraceLog(LOG_ERROR, "Não há cartas para comprar!");
			return;
		}
	}

	Pilha* noDoBaralho = *baralho; // O nó do topo do baralho

	// Criar novo nó para a mão
	CartaNode* nova_mao = create_mao();
	if (!nova_mao) return; // Falha de alocação
	
	nova_mao->carta = noDoBaralho->carta;

	nova_mao->carta = noDoBaralho->carta; // A mão aponta para a *mesma* carta
	nova_mao->next = (*jogador_da_vez)->mao; // Adiciona ao início da mão
	(*jogador_da_vez)->mao = nova_mao;

	// Avançar o baralho
	*baralho = (*baralho)->next;

	free(noDoBaralho); // Libera o *nó* da pilha (baralho)
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
	// A UI passa um índice 1-based (começando em 1)
	int indice_base_0 = carta_selecionada - 1;

	// --- Validação ---
	if (!player || player->mao == NULL) {
		TraceLog(LOG_WARNING, "jogar_pilha: Jogador nulo ou mão vazia.");
		return NULL;
	}

	if (indice_base_0 < 0 || indice_base_0 >= count_mao(player->mao)) {
		TraceLog(LOG_ERROR, "jogar_pilha: Indice invalido %d (total de cartas: %d)", indice_base_0, count_mao(player->mao));
		return NULL;
	}

	// --- Lógica de Remoção ---
	CartaNode* no_para_remover = NULL;
	Carta* carta_jogada = NULL;

	// Caso 1: Removendo o primeiro nó (a cabeça da lista)
	if (indice_base_0 == 0) {
		no_para_remover = player->mao;      // Salva o nó da cabeça
		player->mao = no_para_remover->next; // Avança a cabeça da lista
	} 
	// Caso 2: Removendo do meio ou do fim
	else {
		// Encontra o nó ANTERIOR ao que queremos remover
		CartaNode* anterior = player->mao;
		for (int i = 0; i < indice_base_0 - 1; i++) {
			if (anterior->next == NULL) {
				// Isso não deve acontecer por causa da checagem de 'total_cartas'
				TraceLog(LOG_ERROR, "jogar_pilha: Atingiu fim da lista inesperadamente.");
				return NULL;
			}
			anterior = anterior->next;
		}

		no_para_remover = anterior->next;       // Este é o nó para remover
		anterior->next = no_para_remover->next; // "Pula" o nó a ser removido
	}

	// --- Adicionar à Pilha de Descarte ---
	if (no_para_remover == NULL) {
		TraceLog(LOG_ERROR, "jogar_pilha: no_para_remover é NULL, lógica falhou.");
		return NULL;
	}

	Pilha* nova_pilha_item = malloc(sizeof(Pilha));
	if (nova_pilha_item == NULL) {
		TraceLog(LOG_ERROR, "jogar_pilha: Falha ao alocar memória para item da pilha.");
		// A carta foi removida da mão, mas não foi para a pilha (vazamento de memória)
		// Isso é ruim, mas pelo menos não vai crashar.
		free(no_para_remover); // Libera o nó
		return NULL;
	}

	carta_jogada = no_para_remover->carta; // Pega a carta
	nova_pilha_item->carta = carta_jogada; // Coloca a carta no novo nó da pilha
	nova_pilha_item->next = game.pilha;
	game.pilha = nova_pilha_item;

	// Finalmente, libera o NÓ da mão que foi removido
	// (NÃO libera a carta, pois ela está na pilha agora)
	free(no_para_remover);

	return carta_jogada; // Retorna a carta que foi jogada
}

#pragma endregion CartaNodePilha


// Baralho, Pilha
#pragma region BaralhoPilha


#pragma endregion BaralhoPilha


// Pilha, Baralho 
#pragma region PilhaBaralho


#pragma endregion PilhaBaralho



// Aplica efeitos especiais das cartas
void aplicar_efeito_carta(PlayerNode* player, Carta* carta) {
	switch (carta->valor) {
		case MAIS_2:
			// +2 s� aplica efeito se N�O for INCOLOR (cartas coridas)
			if (carta->cor != INCOLOR) {
				game.comprar_cartas += 2;
			}
			break;

		case MAIS_4:
			// +4 sempre aplica efeito (s� existe em INCOLORs)
				comprar_cartas += 4;
				// chamar tela +4
			break;

		case REVERSO:
			game.direcao = !game.direcao;
			break;

		case BLOQUEAR:
			game.jogador_bloqueado = 1;
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