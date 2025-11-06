#include "game-manager.h"
#include "card-node.c"
#include "raylib.h"

typedef enum {
	RIGHT,
	LEFT
} Direction;

// Structs
struct PlayerList;

// Vars
static const cartas = 28;

static CardNode* baralho = NULL;
static CardNode* pilha = NULL;
static CardNode* p1 = NULL;
static CardNode* p2 = NULL;
static CardNode* p3 = NULL;
static CardNode* p4 = NULL;

static PlayerList* lista_jogadores = NULL;

static Direction direcao = RIGHT;

// Aux Functions

typedef struct {
	CardNode** player;
	CardNode** prev;
	CardNode** next;
} PlayerList;

// Init
void definir_baralho() 
	CardNode* novo = malloc(sizeof(CardNode));
	for (int i = 0; i < cartas; i++) {
		novo->card->valor = GetRandomValue(0, 15);
		novo->card->cor = GetRandomValue(0, 4);
		add_card(&baralho, novo->card);
		novo = novo->next;
	}
}

void iniciar_baralho(CardNode **baralho) {
	definir_baralho();
	
}

void iniciar_partida() {
	iniciar_baralho();
}

// DeInit
void acabarJogo() {
	ganhador();
}

void descarregar_partida() {}

// Update
void atualizar_partida() {}

// Draw
void desenhar_partida() {}

