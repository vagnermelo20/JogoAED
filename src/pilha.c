#include "pilha.h"
#include "stdlib.h"
#include "raylib.h"
#include "time.h"


// Generalized Functions
int count(Pilha* pilha) {
	TraceLog(LOG_INFO, "pop");
	int count = 0;
	if (!pilha || pilha == NULL) return 0;
	Pilha* aux = pilha;
	while (aux->next != NULL) {
		aux = aux->next;
		count++;
	} 
	return count;
}




Carta* remover_topo(Pilha** pilha) {
	if (!pilha || !*pilha) return NULL;
	Pilha* aux = *pilha;
	Carta* card = aux->carta;
	*pilha = aux->next;
	free(aux);
	return card;
}

void adicionar_carta(Pilha *baralho, Carta* carta) {
	if (baralho || !carta) return;
	Pilha* novo = malloc(sizeof(Pilha));
	novo->next = baralho;
	novo->carta = carta;
	
	baralho = novo; 
}


void unload_all_pilha(Pilha** head) {

}

// Baralho
void initialize_baralho(Pilha* baralho, int num_cartas) {
	if (num_cartas == 0 || !baralho) {
		return;
	}
	for (int i = 0; i < num_cartas; i++) {
		Pilha* nova = (Pilha*)malloc(sizeof(Pilha));
		Carta* carta = (Carta*)malloc(sizeof(Carta));

		if (nova == NULL || carta == NULL) {
			if (nova) free(nova);
			if (carta) free(carta);
			return;
		}
		carta->valor = GetRandomValue(0,9);
		carta->cor = GetRandomValue(1, 4);

		nova->carta = carta;
		nova->next = baralho;
		baralho = nova;
	}
}


// Pilha 
Carta* check_top(Pilha* pilha) {
	if (pilha->carta == NULL) return NULL;
	return (pilha->carta);
}



// Baralho, Pilha
void initialize_pilha(Pilha* pilha, Pilha* baralho) {
	if (pilha != NULL || !baralho) {
		return;
	}
	pilha = (Pilha*)malloc(sizeof(Pilha));
	Pilha* aux = baralho;
	baralho = baralho->next;
	pilha = aux;
	pilha->next = NULL;
}


// Pilha, Baralho
void refill(Pilha** pilha, Pilha** baralho) {
	if (!(*pilha) || !pilha || !(*baralho) || !baralho) return;
	*baralho = (*pilha)->next;
	(*pilha)->next = NULL;
	embaralhar(pilha, baralho);
}

void embaralhar(Pilha *pilha, Pilha* baralho) {
	if (count(baralho) <= 1) return;

	// Converter pilha para array
	Carta* cartas = (Carta*)malloc(sizeof(Carta) * count(baralho));
	int count = 0;

	while (remover_topo(baralho) != NULL) {
		refill(pilha, baralho);
	}

	// Embaralhar array
	srand(time(NULL));
	for (int i = count - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		Carta temp = cartas[i];
		cartas[i] = cartas[j];
		cartas[j] = temp;
	}

	// Reconstruir pilha
	for (int i = 0; i < count; i++) {
		adicionar_carta(baralho, &cartas[i]);
	}

	free(cartas);
}
 
#pragma endregion 

