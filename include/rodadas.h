#pragma once

#include "stdlib.h"
#include "player.h"
#include "card.h"
#include "pilha.h"

int rodada_atual = 0;
Player *head = (Player*)malloc(sizeof(Player));
Player *atual = head;

void iniciar_partida() 
	quantidade_players();
	return;
}
	

int rodada_atual(){
	
}

void acabar_jogo() {
	ganhador();
}