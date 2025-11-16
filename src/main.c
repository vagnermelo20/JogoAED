#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "carta.h"
#include "pilha.h"
#include "player.h"

#include "game_manager.h"
#include "render_game.h"

#define MAX_CARTAS_UI 20

// Protótipos de funções
void processarTurnoJogador(CartaUI* cartasUI, int numCartasUI, Assets* assets, int* aguardandoEscolhaCor, Carta** cartaPendenteEfeito);
void processarTurnoCPU();

int main() {
    InitWindow(LARGURA_TELA, ALTURA_TELA, "UNO - Jogo de Cartas");
    SetTargetFPS(60);

    srand(time(NULL));

    Assets assets = carregarAssets();
    initialize_game(4);

    CartaUI cartasUI[MAX_CARTAS_UI];
    int numCartasUI = 0;
    int aguardandoEscolhaCor = 0;
    Carta* cartaPendenteEfeito = NULL;
    Texture2D background = LoadTexture("assets/background_2.png");

    PlayerNode* jogadorHumano = game.jogador_da_vez;

    // Loop principal
    while (!WindowShouldClose()) {
        verificar_vitoria();

        if (game.jogoTerminado) {
            BeginDrawing();
            DrawTexture(background, 0, 0, WHITE);
            desenharInfoJogo(&assets);
            desenharPilhaJogo(&game.pilha, &assets, game.corAtual);
            Vector2 posBaralho = { LARGURA_TELA / 2 + 60, ALTURA_TELA / 2 - ALTURA_CARTA / 2 };
            desenharBaralho(game.baralho, &assets, posBaralho);
            int numCartas;
            desenharMaoJogador(jogadorHumano->mao, &assets, cartasUI, &numCartas);
            
            if (game.vencedor != NULL) {
                DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.7f));
                char mensagem[100];
                sprintf(mensagem, "p%d VENCEU!", game.vencedor->numero);
                int larguraTexto = MeasureText(mensagem, 50);
                DrawText(mensagem, LARGURA_TELA / 2 - larguraTexto / 2, ALTURA_TELA / 2 - 50, 50, GOLD);
                DrawText("Pressione ESC para sair", LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 + 20, 25, WHITE);
            }
            EndDrawing();
            continue;
        }

        desenharMaoJogador(jogadorHumano->mao, &assets, cartasUI, &numCartasUI);

        if (game.jogador_da_vez == jogadorHumano) {
            if (aguardandoEscolhaCor) {
                if (IsKeyPressed(KEY_ONE)) {
                    game.corAtual = AMARELO;
                    aguardandoEscolhaCor = 0;
                    next_player();
                    
                    if (cartaPendenteEfeito != NULL) {
                        aplicar_efeito_carta(game.jogador_da_vez, cartaPendenteEfeito);
                        cartaPendenteEfeito = NULL;
                    }
                }
                else if (IsKeyPressed(KEY_TWO)) {
                    game.corAtual = AZUL;
                    aguardandoEscolhaCor = 0;
                    next_player();
                    
                    if (cartaPendenteEfeito != NULL) {
                        aplicar_efeito_carta(game.jogador_da_vez, cartaPendenteEfeito);
                        cartaPendenteEfeito = NULL;
                    }
                }
                else if (IsKeyPressed(KEY_THREE)) {
                    game.corAtual = VERDE;
                    aguardandoEscolhaCor = 0;
                    next_player();
                    
                    if (cartaPendenteEfeito != NULL) {
                        aplicar_efeito_carta(game.jogador_da_vez, cartaPendenteEfeito);
                        cartaPendenteEfeito = NULL;
                    }
                }
                else if (IsKeyPressed(KEY_FOUR)) {
                    game.corAtual = VERMELHO;
                    aguardandoEscolhaCor = 0;
                    next_player();
                    
                    if (cartaPendenteEfeito != NULL) {
                        aplicar_efeito_carta(game.jogador_da_vez, cartaPendenteEfeito);
                        cartaPendenteEfeito = NULL;
                    }
                }
            }
            else {
                processarTurnoJogador(cartasUI, numCartasUI, &assets, &aguardandoEscolhaCor, &cartaPendenteEfeito);
            }
        }
        else {
            processarTurnoCPU();
            WaitTime(0.5);
        }

        // Desenhar tudo
        BeginDrawing();
        
        DrawTexture(background, 0, 0, WHITE);
        desenharInfoJogo(&assets);
        desenharPilhaJogo(&game.pilha, &assets, game.corAtual);
        
        Vector2 posBaralho = { LARGURA_TELA / 2 + 60, ALTURA_TELA / 2 - ALTURA_CARTA / 2 };
        desenharBaralho(game.baralho, &assets, posBaralho);
        DrawText("Comprar", posBaralho.x + 10, posBaralho.y - 25, 18, BLACK);
        
        int numCartas;
        desenharMaoJogador(jogadorHumano->mao, &assets, cartasUI, &numCartas);
        
        if (game.jogador_da_vez == jogadorHumano && game.comprar_cartas > 0) {
            DrawRectangle(LARGURA_TELA / 2 - 200, ALTURA_TELA / 2 - 120, 400, 60, Fade(RED, 0.8f));
            char msg[100];
            sprintf(msg, "COMPRE %d CARTAS!", game.comprar_cartas);
            DrawText(msg, LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 - 100, 30, WHITE);
        }
        
        if (game.jogador_da_vez == jogadorHumano && game.jogador_bloqueado) {
            DrawRectangle(LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 - 100, 300, 80, Fade(BLACK, 0.8f));
            DrawText("BLOQUEADO!", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 - 80, 40, RED);
            DrawText("Aguarde...", LARGURA_TELA / 2 - 70, ALTURA_TELA / 2 - 30, 25, WHITE);
        }
        
        if (aguardandoEscolhaCor) {
            DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.5f));
            DrawText("VOCE jogou um INCOLOR!", LARGURA_TELA / 2 - 200, ALTURA_TELA / 2 - 140, 28, GOLD);
            DrawText("VOCE escolhe a cor:", LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 - 100, 30, WHITE);
            DrawText("1 - Amarelo", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 - 50, 25, YELLOW);
            DrawText("2 - Azul", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 - 20, 25, BLUE);
            DrawText("3 - Verde", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 + 10, 25, GREEN);
            DrawText("4 - Vermelho", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 + 40, 25, RED);
        }

        EndDrawing();
    }

    UnloadTexture(background);
    descarregarAssets(&assets);
    liberarJogo();
    CloseWindow();

    return 0;
}

void processarTurnoJogador(CartaUI* cartasUI, int numCartasUI, Assets* assets, int* aguardandoEscolhaCor, Carta** cartaPendenteEfeito) {
    Vector2 mousePos = GetMousePosition();

    if (game.comprar_cartas > 0) {
        Rectangle baralhoRect = { LARGURA_TELA / 2 + 60, ALTURA_TELA / 2 - ALTURA_CARTA / 2,
                                 LARGURA_CARTA, ALTURA_CARTA };

        if (IsKeyPressed(KEY_SPACE) ||CheckCollisionPointRec(mousePos, baralhoRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < game.comprar_cartas; i++) {
                puxar_baralho(&game.jogador_da_vez, &game.baralho);
            }
            game.comprar_cartas = 0;
            next_player();
        }
        return;
    }

    static double tempoBloqueio = 0;
    if (game.jogador_bloqueado) {
        tempoBloqueio += GetFrameTime();
        
        if (tempoBloqueio > 1.0) {
            game.jogador_bloqueado = 0;
            tempoBloqueio = 0;
            next_player();
        }
        return;
    }
    tempoBloqueio = 0;

    Rectangle baralhoRect = { LARGURA_TELA / 2 + 60, ALTURA_TELA / 2 - ALTURA_CARTA / 2,
                             LARGURA_CARTA, ALTURA_CARTA };

    if (IsKeyPressed(KEY_SPACE) ||CheckCollisionPointRec(mousePos, baralhoRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        puxar_baralho(&game.jogador_da_vez, &game.baralho);
        next_player();
        return;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < numCartasUI; i++) {
            if (CheckCollisionPointRec(mousePos, cartasUI[i].bounds)) {
                Carta* cartaSelecionada = cartasUI[i].carta;

                if (validar_jogada(cartaSelecionada, game.pilha)) {
                    Carta* cartaJogada = jogar_pilha(game.jogador_da_vez, i + 1);

                    if (cartaJogada != NULL) {
                        if (cartaJogada->cor == INCOLOR) {
                            *aguardandoEscolhaCor = 1;
                            *cartaPendenteEfeito = cartaJogada;
                            return;
                        }
                    aplicar_efeito_carta(game.jogador_da_vez, cartaJogada);   

                    if (cartaJogada->valor == DOIS) {
                        game.comprar_cartas += 2;
                        return;

                    }
                    if (cartaJogada->valor == TRES) {
                        processarTurnoCPU(); // 3 joga outra carta aletória
                    }
                    if (cartaJogada->valor == QUATRO){
                        CartaNode* maoTemp = game.jogador_da_vez->mao;
                        game.jogador_da_vez->mao = game.jogador_da_vez->prev->mao;
						game.jogador_da_vez->prev->mao = maoTemp; // 4 troca a mão com o jogador anterior

                    }
                    next_player();
                    return;
                    }
                }
                break;
            }
        }
    }
}

void processarTurnoCPU() {
    if (game.comprar_cartas > 0) {
        for (int i = 0; i < game.comprar_cartas; i++) {
            puxar_baralho(&game.jogador_da_vez, &game.baralho);
        }
        game.comprar_cartas = 0;
        next_player();
        return;
    }

    if (game.jogador_bloqueado) {
        game.jogador_bloqueado = 0;
        next_player();
        return;
    }

    CartaNode* cartaAtual = game.jogador_da_vez->mao;
    Carta* melhorCarta = NULL;
    int indiceMelhorCarta = -1;
    int i = 0;

    while (cartaAtual != NULL) {
        if (validar_jogada(cartaAtual->carta, game.pilha)) {
            melhorCarta = cartaAtual->carta;
            indiceMelhorCarta = i;
            
            if (cartaAtual->carta->valor >= MAIS_2) {
                break;
            }
        }
        cartaAtual = cartaAtual->next;
        i++;
    }

    if (melhorCarta != NULL) {
        Carta* cartaJogada = jogar_pilha(game.jogador_da_vez, indiceMelhorCarta + 1);

        if (cartaJogada != NULL) {
            if (cartaJogada->cor == INCOLOR) {
                game.corAtual = (Cor)GetRandomValue(0, 3);
            }

            aplicar_efeito_carta(game.jogador_da_vez, cartaJogada);

            if (cartaJogada->valor == DOIS) {
                puxar_baralho(&game.jogador_da_vez, &game.baralho);
                puxar_baralho(&game.jogador_da_vez, &game.baralho);
            }
            if (cartaJogada->valor == TRES) {
                processarTurnoCPU();
            }
            if (cartaJogada->valor == QUATRO) {
                CartaNode* maoTemp = game.jogador_da_vez->mao;
                game.jogador_da_vez->mao = game.jogador_da_vez->prev->mao;
                game.jogador_da_vez->prev->mao = maoTemp;

            }
            next_player();
            return;
        }
    }

    puxar_baralho(&game.jogador_da_vez, &game.baralho);
    next_player();
}