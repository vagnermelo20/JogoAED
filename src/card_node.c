#include "card_node.h"
#include "raylib.h"
#include "stdlib.h"
#include "carta.h"

int count_mao(CartaNode* mao) {
	int count = 0;
	if (mao == NULL) return 0;
	CartaNode* aux = mao;
	while (aux->next != NULL){
		aux = aux->next;
		count++;
	} 
	return count;
}


CartaNode* create_mao() {
	CartaNode* mao = malloc(sizeof(CartaNode));
	if (mao == NULL) return NULL;
	mao->carta = create_carta();
	if (mao->carta == NULL) {
		free(mao);
		return NULL;
	}
	mao->next = NULL;
	if (mao->next != NULL) {
		free(mao);
		return NULL;
	}
	return mao;
}