#include "carta.h"
#include "stdlib.h"


Carta* create_carta(void) {
	Carta* carta = malloc(sizeof(Carta));
	if (carta == NULL) return NULL;
	carta->valor = 0;
	carta->cor = 0;
	return carta;
}

Carta* create_random_carta(void) {
	Carta* carta = malloc(sizeof(Carta));
	if (carta == NULL) return NULL;
	carta->valor = GetRandomValue(0, 9);
	carta->cor = GetRandomValue(1, 4);
	return carta;
}