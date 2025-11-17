#include "carta.h"
#include "stdlib.h"


Carta* criar_carta(void) { // cria um 0 amarelo que vai usar na mao de cada jogador
	Carta* carta = malloc(sizeof(Carta));
	if (carta == NULL) return NULL;
	carta->valor = 0;
	carta->cor = 0;
	return carta;
}