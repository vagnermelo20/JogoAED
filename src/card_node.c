#include "card_node.h"
#include "raylib.h"
#include "stdlib.h"

int count_mao(CartaNode* mao) {
	int count = 0;
	if (mao == NULL) return count;
	CartaNode* aux = mao;
	do{
		aux = aux->next;
		count++;
	} while (aux != NULL);
	return count;
}


