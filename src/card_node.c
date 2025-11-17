#include "card_node.h"
#include "raylib.h"
#include "stdlib.h"
#include "carta.h"

int contar_mao(CartaNode* mao) {
    int count = 0;
    CartaNode* aux = mao;
    
    while (aux != NULL) {
        count++;
        aux = aux->next;
    }
    
    return count;
}

CartaNode* criar_mao() {
    CartaNode* mao = malloc(sizeof(CartaNode));
    if (mao == NULL) return NULL;
    
    mao->carta = criar_carta(); 
    if (mao->carta == NULL) {
        free(mao);
        return NULL;
    }
    
    mao->next = NULL;
    return mao;
}


int comparar_cartas(Carta* carta1, Carta* carta2) {
    // Ordena por cor e por valor
    if (carta1->cor < carta2->cor) return -1;
    if (carta1->cor > carta2->cor) return 1;
    
    // Se cores iguais, compara por VALOR
    if (carta1->valor < carta2->valor) return -1;
    if (carta1->valor > carta2->valor) return 1;
    
    return 0; // Cartas idênticas
}


void ordenar_mao(CartaNode** head) { //bubblesort v1
    if (*head == NULL || (*head)->next == NULL) return;
    
    int len = contar_mao(*head);
    
    for (int i = 1; i <= len - 1; i++) {
        CartaNode* atual = *head;
        
        for (int j = len - 1; j >= i; j--) {
            // Navegar até a posição j e depois pro j - 1, pra frazer a troca
            CartaNode* node_j = *head;
            for (int k = 0; k < j; k++) {
                node_j = node_j->next;
            }
            
            CartaNode* node_j_prev = *head;
            for (int k = 0; k < j - 1; k++) {
                node_j_prev = node_j_prev->next;
            }
            
            if (comparar_cartas(node_j->carta, node_j_prev->carta) < 0) {
                Carta* aux = node_j->carta;
                node_j->carta = node_j_prev->carta;
                node_j_prev->carta = aux;
            }
        }
    }
}