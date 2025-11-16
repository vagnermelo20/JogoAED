#include "card_node.h"
#include "raylib.h"
#include "stdlib.h"
#include "carta.h"

int count_mao(CartaNode* mao) {
    int count = 0;
    CartaNode* aux = mao;
    
    while (aux != NULL) {
        count++;
        aux = aux->next;
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
    return mao;
}


int comparar_cartas(Carta* carta1, Carta* carta2) {
    // Primeiro ordena por COR
    if (carta1->cor < carta2->cor) return -1;
    if (carta1->cor > carta2->cor) return 1;
    
    // Se cores iguais, ordena por VALOR
    if (carta1->valor < carta2->valor) return -1;
    if (carta1->valor > carta2->valor) return 1;
    
    return 0; // Cartas idênticas
}

void bubble_sort_mao(CartaNode** head) {
    if (*head == NULL || (*head)->next == NULL) return;
    
    int len = count_mao(*head);
    
    // Linha 5: for(int i = 1; i <= LEN-1; i++)
    for (int i = 1; i <= len - 1; i++) {
        CartaNode* atual = *head;
        
        // Linha 6: for(int j = LEN-1; j >= i; j--)
        for (int j = len - 1; j >= i; j--) {
            // Navegar até a posição j
            CartaNode* node_j = *head;
            for (int k = 0; k < j; k++) {
                node_j = node_j->next;
            }
            
            CartaNode* node_j_prev = *head;
            for (int k = 0; k < j - 1; k++) {
                node_j_prev = node_j_prev->next;
            }
            
            // Linha 7: if (v[j] < v[j-1])
            if (comparar_cartas(node_j->carta, node_j_prev->carta) < 0) {
                // Linhas 8-10: Trocar cartas
                Carta* aux = node_j->carta;
                node_j->carta = node_j_prev->carta;
                node_j_prev->carta = aux;
            }
        }
    }
}


void ordenar_mao(CartaNode** mao){

	if (mao != NULL){

		bubble_sort_mao(mao);
	}
}