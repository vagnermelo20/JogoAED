#pragma once
#include "raylib.h"
#include "render.h"

// Desenha a tela de vencedor. Se o usuário clicar em 'Jogar novamente', *playAgain será setado para 1.
// Usa a textura de background para manter consistência visual com as outras telas.
void DrawTelaGanhou(Assets* assets, Texture2D background, const char* vencedorNome, int* playAgain);

