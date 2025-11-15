#pragma once

typedef enum Cor {
	INCOLOR,
	AMARELO,
	AZUL,
	VERDE,
	VERMELHO
} Cor;

typedef enum Valor {
	ZERO,
	UM,
	DOIS,
	TRES,
	QUATRO,
	CINCO,
	SEIS,
	SETE,
	OITO,
	NOVE,
	MAIS_2,
	MAIS_4,
	DEFINIR_COR,
	BLOQUEAR_ANTERIOR,
	BLOQUEAR,
	REVERSO
} Valor;

typedef struct Carta {
	Cor cor;
	Valor valor;
} Carta;