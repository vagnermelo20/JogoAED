#include "pilha.h"
#include "stdlib.h"
#include "raylib.h"
#include "time.h"


// Generalized Functions
int count(Pilha* pilha) {
	
	int count = 0;
	if (!pilha || pilha == NULL) return 0;
	Pilha* aux = pilha;
	while (aux != NULL) {
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

void adicionar_carta(Pilha** pilha, Carta carta) {
	Pilha* novo = malloc(sizeof(Pilha));
	if (novo == NULL) {
		return;
	}

	novo->carta = malloc(sizeof(Carta));
	if (novo->carta == NULL) {
		free(novo);
		return;
	}

	*(novo->carta) = carta;
	novo->next = *pilha;
	*pilha = novo;
}



void unload_all_pilha(Pilha** head) {
	// Implementar a liberação de memória
	Pilha* atual = *head;
	Pilha* proximo;
	while (atual != NULL) {
		proximo = atual->next;
		free(atual->carta); // Libera a carta
		free(atual);        // Libera o nó da pilha
		atual = proximo;
	}
	*head = NULL; // Define o ponteiro original como NULL
}

// Baralho
void initialize_baralho(Pilha** baralho, int num_cartas) {
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
		carta->valor = GetRandomValue(0, 9);
		carta->cor = GetRandomValue(1, 4);

		nova->carta = carta;
		nova->next = *baralho;
		*baralho = nova;
	}
}


// Pilha 
Carta* check_top(Pilha* pilha) {
	if (pilha == NULL || pilha->carta == NULL) return NULL;
	return (pilha->carta);
}



// Baralho, Pilha
void initialize_pilha(Pilha** pilha, Pilha** baralho) {
	if (baralho == NULL || *baralho == NULL) {
		return;
	}

	Pilha* topo = *baralho;
	*baralho = (*baralho)->next;

	topo->next = *pilha;
	*pilha = topo;
}

	

// Pilha, Baralho
void refill(Pilha** pilha, Pilha** baralho) { // Ao verificar um baralho zerado, salva o topo da pilha e junta o resto ao baralho
	if (!pilha || !(*pilha) || !baralho) return;
	Pilha* topo = *pilha;
	*pilha = (*pilha)->next;
	topo->next = NULL;

	*baralho = *pilha;

	*pilha = topo;

	if (*baralho != NULL) {
		embaralhar(baralho, pilha);
	}
}

void embaralhar(Pilha** pilha, Pilha **baralho) {
	int n = count(*pilha);
	if (n <= 1) {
		return;
	}

	Carta** array_cartas = malloc(n * sizeof(Carta*));
	if (array_cartas == NULL) {
		return;
	}

	Pilha* temp = *pilha;
	for (int i = 0; i < n; i++) {
		array_cartas[i] = temp->carta;
		temp = temp->next;
	}

	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		Carta* temp_carta = array_cartas[i];
		array_cartas[i] = array_cartas[j];
		array_cartas[j] = temp_carta;
	}

	temp = *pilha;
	for (int i = 0; i < n; i++) {
		temp->carta = array_cartas[i];
		temp = temp->next;
	}

	free(array_cartas);
}
 
#pragma endregion 

