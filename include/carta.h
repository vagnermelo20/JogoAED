#pragma once

typedef enum Cor {
    INCOLOR,   // 0 - NULL / SPECIAL
    AMARELO,   // 1 - yellow
    AZUL,      // 2 - blue
    VERDE,     // 3 - green
    VERMELHO  // 4 - red
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
Carta* criar_carta();