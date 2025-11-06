#pragma once

#include "card.h"

typedef struct Player {
	char* nome;
	int rodadas;
} Player;



int quantidade_players();

Player pularJogador(Player atual);

Player jogadorGanhador(Player atual);
