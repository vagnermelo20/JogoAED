#pragma once

typedef enum Valor {
	ZERO,
	UM,
	DOIS,
	TRES,
	QUATRO,
	CINCO,
	SEIS,
	SETE,
	OIT0,
	NOVE,
	BLOQUEAR,
	BLOQUEAR_ANTERIOR,
	DEFINIR_COR,
	MAIS_2,
	MAIS_4,
	REVERSO
} Valor;

typedef enum Cor {
	INCOLOR,
	AMARELO,
	AZUL,
	VERDE,
	VERMELHO
} Cor;

typedef struct Card {
	Valor valor;
	Cor cor;
} Card;