#pragma once
#include "raylib.h"

typedef enum MenuState {
    MENU_PRINCIPAL,
    MENU_INSTRUCOES,
    MENU_JOGAR
} MenuState;

// Desenha o menu e retorna o estado atual
MenuState DrawMenu(Texture2D background);

// Desenha a tela de instruções e retorna quando o usuário voltar
MenuState DrawInstrucoes(Texture2D background);