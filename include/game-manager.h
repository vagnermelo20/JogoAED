#pragma once

#include "stdlib.h"
#include "player.h"
#include "card.h"
#include "baralho.h"

void iniciar_jogo();
void distribuir_cartas(Player** players, Baralho** baralho, int num_cartas);
Card* comprar_carta(Baralho** baralho);
void jogar_carta(Player** jogador, Baralho** baralho, Card* carta);
int jogador_vencedor(Player* players);
