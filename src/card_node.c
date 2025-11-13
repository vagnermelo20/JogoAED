#include "card_node.h"
#include "raylib.h"


int count_node(CardNode* head) {
	if (head == NULL) {
        return 0;
    }
	CardNode* aux = head;
	int count = 0;
	do{
		head=head->next;
		count ++;
	}
	while (head != NULL);
	return count;
}


void add_card_hand(CardNode** head, Card* card) {
	if (!card) return;

	CardNode* novo = malloc(sizeof(CardNode));
	if(!novo) return;

	novo->card = card;

	if (*head == NULL){

		*head = novo;
		(*head)->next = NULL;
	}
	else{
		novo->next = *head;
		*head = novo;
	}
}

void remove_card_hand(CardNode** cardNode, Card* card) {
	if (!card || !cardNode || !*cardNode) return;

	CardNode* aux = *cardNode;

	if (aux->card == card) {
		*cardNode = aux->next;
		return;
	}

	while (aux->next != NULL) {
		if (aux->next->card == card) {
			CardNode* temp = aux->next;
			aux->next = temp->next;
			return;
		}
		aux = aux->next;
	}
}


