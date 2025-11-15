#pragma once

#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"
#include "game_manager.h"

#define LARGURA_TELA 1280
#define ALTURA_TELA 720
#define LARGURA_CARTA 100
#define ALTURA_CARTA 140

typedef struct {
    Texture2D cartasNumericas[4][10];  // [cor][numero]
    Texture2D cartasEspeciais[4][3];   // [cor][tipo: 0=+2, 1=reverse, 2=skip]
    Texture2D cartasIncolor;
    Texture2D cartaIncolorMaisQuatro;
    Texture2D cartaVerso;
    Font fonte;
} Assets;

typedef struct CartaUI{
    Rectangle bounds;
    Carta* carta;
    int selecionada;
} CartaUI;

// Funções de renderização
Assets carregarAssets();
void descarregarAssets(Assets* assets);
void desenharTela(Assets* assets, CartaUI* cartasJogador, int numCartasJogador, Texture2D background, Player* jogadorHumano);
void desenharCarta(Carta* carta, Vector2 posicao, Assets* assets, int mostrarFrente);
void desenharMaoJogador(CartaNode* mao, Assets* assets, CartaUI* cartasUI, int* numCartas);
void desenharPilhaJogo(Pilha** pile, Assets* assets, Cor corAtual);
void desenharBaralho(Pilha* pilha, Assets* assets, Vector2 posicao);
void desenharInfoJogo(Assets* assets);
const char* obterNomeCor(Cor cor);
const char* obterNomeValor(Valor valor);

#endif // RENDER_H