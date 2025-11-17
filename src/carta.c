#include "carta.h"
#include "stdlib.h"


Carta* criar_carta(void) { // aloca memória para uma carta com valores zerados
	Carta* carta = malloc(sizeof(Carta));
	if (carta == NULL) return NULL;
	carta->valor = 0;
	carta->cor = 0;
	return carta;
}