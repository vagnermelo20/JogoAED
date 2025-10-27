#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Carta
{
    char CartaReal; // Será um char por ser uma carta
    int valorAleatorio;
    struct Carta *prox;
    struct Carta *ant;
} Carta;


void adicionarCartaInicial(Carta **head, Carta **tail, char CartaReal, int valorAleatorio){

    Carta *novo = (Carta *)malloc(sizeof(Carta));
    if (novo != NULL){

        novo->CartaReal = CartaReal;
        novo->valorAleatorio = valorAleatorio;

        if(*head == NULL){

            *head = *tail = novo;
            novo->prox = NULL;
            novo->ant = NULL;

        }

        else{
            novo->prox = *head;
            *head = novo;
            novo->ant = NULL;
        }
    }
}


// funções auxiliares para poder usar o sort;
void preencherArray(Carta *head, int arrayCartas[]){
    int i = 0;
    while (head != NULL){

        arrayCartas[i] = head->valorAleatorio;
        head = head->prox;
        i++; 

    } 
}

int contarTamanho(Carta *head, int arrayCartas[]){ // Separado da função preencher array pra separar as funcionalidades.
    int tamanho = 0;
    while (head != NULL){
        head = head->prox;
        tamanho++; 
    }    
    return tamanho;
}

void ordenarBubblesortV1(int arrayCartas[], int tamanho) {
    for (int i = 1; i <= tamanho - 1; i++) {
        for (int j = tamanho - 1; j >= i; j--) {
            if (arrayCartas[j] < arrayCartas[j - 1]) {
                int aux = arrayCartas[j];
                arrayCartas[j] = arrayCartas[j - 1];
                arrayCartas[j - 1] = aux;
            }
        }
    }
}

// Após ordenar no Bubblesort, nós iremos alterar a lista encadeada pra ela possuir a nova ordem;
// Nós faremos isso criando uma segunda head após o tail1 estando ordenada e nós criaremos também uma segunda tail no final,
// Por último nós excluiremos tudo antes da segunda head;
void adicionarCartaOrdenadas(Carta **head, Carta **tail, Carta **head2, Carta *tail2, int arrayCartas[]){

    while(*head != NULL){
        
        Carta *novo = (Carta *)malloc(sizeof(Carta));
        if (novo != NULL){
            novo = acharCartaValorAleatorio(int arrayCartas[], &head, int tamanho);
            adicionarCartaFinal(&tail, &head2, &tail2, novo);  
            removerCartaPorValorAleatorio(&head, &tail, &head2, int valorAleatorio);
        }
    }
}



// função auxiliar para achar valor dentro de um array.
Carta *acharCartaValorAleatorio(int arrayCartas[], Carta *head, int tamanho){
    if (head != NULL){
        if(head->valorAleatorio == arrayCartas[i]){
            return head;
        }
    }
    return NULL;
}

// função pra remover a carta da lista encadeada no começo dela.
void removerCartaPorValorAleatorio(Carta **head, Carta **tail, Carta **head2, int valorAleatorio){

    if(*head != NULL){

        if(*head == *tail){
            if(valorAleatorio == (*head)->valorAleatorio){

                free(*head);
                *head = NULL;
                *tail = NULL;
                (*head2)->ant = NULL;
            }    
        }
        else if(valorAleatorio == (*head)->valorAleatorio){

            Carta *aux = *head;
            *head = (*head->prox);
            (*head)->ant = NULL;
            free (aux);
        }

        else if (valorAleatorio == (*tail)->valorAleatorio){

            Carta *aux = *tail;
            *tail = (*tail)->ant;
            (*tail)->prox = *head2;
            free(aux);
        }
        else{

            Carta *aux = *head;
            while(valorAleatorio != aux->valorAleatorio && aux != NULL){

                aux = aux->prox;
            }
            if(valorAleatorio == aux->valorAleatorio){

                aux->ant->prox = aux->prox;
                aux->prox->ant = aux->ant;
                free(aux);
            }
        }
    }
}

void adicionarCartaFinal(**tail, **head2, **tail2, **novaCarta){

    if (*tail != NULL){
        (*novaCarta)->prox = NULL;
        if ((*tail)->prox == NULL){

            (*tail)->prox = *novaCarta;
            (*novaCarta)->ant = *tail;
            *head2 = *tail2 = novaCarta;
        }
        else{

            (*tail2)->prox = *novaCarta;
            (*novaCarta)->ant = *tail2;
            *tail2 = *novaCarta
        }
    }
}