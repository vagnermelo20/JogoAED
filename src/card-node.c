#include "card-node.h"
#include "stdlib.h"

int count(CardNode* cardNode) {
	CardNode* aux = cardNode;
	int count = 0;
	while (aux != NULL) {
		count++;
		aux = aux->next;
	}
	return count;
}

void add_card(CardNode** cardNode, Card* card) {
	if (!card) return;

	CardNode* novo = malloc(sizeof(CardNode));
	if(!novo) return;

	novo->card = card;
	novo->next = *cardNode;
	*cardNode = novo;
}

void remove_card(CardNode** cardNode, Card* card) {
	if (!card || !cardNode || !*cardNode) return;
	
	CardNode* aux = *cardNode;
	
	if (aux->card == card) {
		*cardNode = aux->next;
		free(aux);
		return;
	}

	while (aux->next != NULL) {
		if (aux->next->card == card) {
			CardNode* temp = aux->next;
			aux->next = temp->next;
			free(temp);
			return;
		}
		aux = aux->next;
	}
}

Card* remove_first(CardNode** cardNode) {
	if (!cardNode || !*cardNode) return NULL;
	CardNode* aux = *cardNode;
	Card* card = aux->card;
	*cardNode = aux->next;
	free(aux);
	return card;
}


