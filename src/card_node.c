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