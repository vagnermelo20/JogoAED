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

			if (j == hand_size - 1) {
 			   ordenar_mao(&game.jogador_da_vez->mao);
			}
		}
		game.jogador_da_vez = game.jogador_da_vez->next;
	}
}



PlayerNode* create_player(CartaNode* mao) {
    PlayerNode* newPlayer = malloc(sizeof(PlayerNode));
    newPlayer->mao = mao = criar_mao();
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



// Inicializa o estado do jogo

void initialize_game(int num_players) {
	create_player_list();
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
	if (game.direcao == 0) {
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
		// Usar contar_mao (de card_node.c)
		if (contar_mao(temp->mao) == 0) {
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



// CartaNode, Baralho
#pragma region PlayerNodeBaralho

void puxar_baralho(PlayerNode** jogador_da_vez, Pilha** baralho) {
	if (!jogador_da_vez || !*jogador_da_vez || !baralho) {
		return;
	}
	if (*baralho == NULL || count(*baralho) == 0) {
		refill(&game.pilha, baralho);
	}

	Pilha* noDoBaralho = *baralho; // O nó do topo do baralho

	CartaNode* nova_mao = criar_mao();
	if (!nova_mao) return; // Falha de alocação
	
	nova_mao->carta = noDoBaralho->carta;

	nova_mao->carta = noDoBaralho->carta; // A mão aponta para a *mesma* carta
	nova_mao->next = (*jogador_da_vez)->mao; // Adiciona ao início da mão
	(*jogador_da_vez)->mao = nova_mao;

	ordenar_mao(&(*jogador_da_vez)->mao);


	// Avançar o baralho
	*baralho = (*baralho)->next;

	free(noDoBaralho); // Libera o *nó* da pilha (baralho)
	return;
}

void puxar_2_cartas(PlayerNode** jogador_da_vez, Pilha** baralho) {
	if (!*jogador_da_vez || !jogador_da_vez || !*baralho || !baralho || !((*baralho)->carta)) return;;
	for (int i = 0; i < 2; i++) puxar_baralho(jogador_da_vez, baralho);
	return;
}

void puxar_4_cartas(PlayerNode** jogador_da_vez, Pilha** baralho) {
	if (!*jogador_da_vez || !jogador_da_vez || !*baralho || !baralho || !((*baralho)->carta)) return;
	for (int i = 0; i < 4; i++) puxar_baralho(jogador_da_vez, baralho);
	return;
}

#pragma endregion


// CartaNode, Pilha
#pragma region CartaNodePilha

int validar_jogada(Carta* carta, Pilha* pilha) {
    if (carta == NULL || pilha == NULL || pilha->carta == NULL) {
        return 0;
    }

    Carta* topoPilha = pilha->carta;

    if (carta->cor == INCOLOR) {
        return 1;
    }

    // Verificar se a cor combina
    if (carta->cor == topoPilha->cor) {
        return 1;
    }

    if (topoPilha->cor == INCOLOR && carta->cor == game.corAtual) {
        return 1;
    }

    // Verificar se o valor combina
    if (carta->valor == topoPilha->valor) {
        return 1;
    }

    return 0;
}

// ...existing code...



Carta* jogar_pilha(PlayerNode* player, int carta_selecionada) {
	int indice_base_0 = carta_selecionada - 1;

	// --- Validação ---
	if (!player || player->mao == NULL) {
		return NULL;
	}

	if (indice_base_0 < 0 || indice_base_0 >= contar_mao(player->mao)) {
		return NULL;
	}

	// --- Lógica de Remoção ---
	CartaNode* no_para_remover = NULL;
	Carta* carta_jogada = NULL;

	if (indice_base_0 == 0) {
		no_para_remover = player->mao;      // Salva o nó da cabeça
		player->mao = no_para_remover->next; // Avança a cabeça da lista
	} 
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

		free(no_para_remover); // Libera o nó
		return NULL;
	}

	carta_jogada = no_para_remover->carta; 
	nova_pilha_item->carta = carta_jogada; 
	nova_pilha_item->next = game.pilha;
	game.pilha = nova_pilha_item;
	free(no_para_remover);

	return carta_jogada; // Retorna a carta que foi jogada
}

#pragma endregion CartaNodePilha


#pragma region BaralhoPilha


#pragma endregion BaralhoPilha


#pragma region PilhaBaralho


#pragma endregion PilhaBaralho



void aplicar_efeito_carta(PlayerNode* player, Carta* carta) {
	switch (carta->valor) {
		case MAIS_2:
			if (carta->cor != INCOLOR) {
				game.comprar_cartas += 2;
			}
			break;

		case MAIS_4:
				game.comprar_cartas += 4;
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

void liberarJogo() {

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

	while (game.baralho != NULL) {
		Pilha* temp = game.baralho;
		game.baralho = temp->next;
		free(temp);
	}

	free(game.baralho);

	while (game.pilha != NULL) {
		Pilha* temp = game.pilha;
		game.pilha = temp->next;
		free(temp);
	}
	free(game.pilha);
}