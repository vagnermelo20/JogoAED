#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Jogador

{
    char nome[21];
    struct Jogador *prox;
    struct Jogador *ant;
    
} Jogador;

void criarUnicoJogador(Jogador **head, Jogador **tail, Jogador **atual, char *nome){

    Jogador *novo = (Jogador *)malloc(sizeof(Jogador));
    if (novo != NULL){

        while (strlen(nome)>= 21){

            printf("Coloque o nome ematé 20 caracteres\n"); //Isso é só pra ter noção de código, depois será transferido pra dentro da raylib e retirado o print;
        }
        strcpy(novo->nome, nome);


        if(*head == NULL){

            *head = *tail = *atual = novo->prox = novo->ant = novo;
        }

        else{

            (*tail)->prox = novo;
            novo->ant = *tail;
            (*head)->ant = novo;
            novo->prox = *head;
            *tail = novo;
        }
    }
}

void andarJogador(Jogador **atual){
    if (*atual != NULL){

        *atual = (*atual)->prox;
    }
}

void removerJogadorEAndar(Jogador **head, Jogador **tail, Jogador **atual){ //Quando usar esse, não será usado o andarJogador;

    if (*atual != NULL){
        Jogador *aux = *atual;

        if((*atual)->prox == *atual){

            free(*atual);
            *atual = NULL;
            *head = NULL;
            *tail = NULL;
        }

        else if(*atual == *tail){
            *tail = (*tail)->ant;
            (*tail)->prox = *head;
            (*head)->ant = *tail;
            *atual = *head;
            free(aux);
        }

        else if (*atual == *head){
            *head = (*head)->prox;
            (*tail)->prox = *head;
            (*head)->ant = *tail;        
            *atual = *head;
            free(aux);
        }
        else{

            (*atual)->prox->ant = (*atual)->ant;
            (*atual)->ant->prox = (*atual)->prox;
            *atual = (*atual)->prox;
            free(aux);
        }
    }
}