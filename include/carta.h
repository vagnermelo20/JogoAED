#pragma once

typedef enum Cor {
    AMARELO,   // 0 - yellow
    AZUL,      // 1 - blue
    VERDE,     // 2 - green
    VERMELHO,  // 3 - red
    INCOLOR    // 4
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
    BLOQUEAR,
    REVERSO
} Valor;

typedef struct Carta {
    Cor cor;
    Valor valor;
} Carta;

//Init
Carta* create_carta();
Carta* create_random_carta();