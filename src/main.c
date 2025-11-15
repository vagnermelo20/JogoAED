#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "carta.h"
#include "bot.h"
#include "pilha.h"
#include "player.h"


#include "game_manager.h"
#include "render_game.h"

#define MAX_CARTAS_UI 20

// Prot�tipos de fun��es
void processarTurnoJogador(CartaUI* cartasUI, int numCartasUI, Assets* assets);
void processarTurnoCPU();
Cor escolherCorINCOLOR(int ehJogadorHumano);
void verificarVitoria();

int main() { 
    // Inicializar Raylib
    InitWindow(LARGURA_TELA, ALTURA_TELA, "UNO - Jogo de Cartas");
    SetTargetFPS(60);

    // Carregar assets
    Assets assets = carregarAssets();

    // Inicializar jogo (1 jogador humano + 3 CPUs)
    inicializarJogo(4);

    // UI
    CartaUI cartasUI[MAX_CARTAS_UI];
    int numCartasUI = 0;
    int aguardandoEscolhaCor = 0;
    Texture2D background = LoadTexture("assets/background_2.png");

    // Guardar refer�ncia do jogador humano
    Player* jogadorHumano = game.jogador_da_vez;

    // Loop principal
    while (!WindowShouldClose()) {
        // Verificar vit�ria
        verificarVitoria();

        if (game.jogoTerminado) {
            desenharTela(&assets, cartasUI, numCartasUI, background, jogadorHumano);
            continue;
        }

        // Atualizar cartas UI do jogador humano sempre
        desenharMaoJogador(jogadorHumano, &assets, cartasUI, &numCartasUI);

        // Processar turno
        if (strcmp(game.jogador_da_vez->nome, "Voc�") == 0) {
            // Turno do jogador humano
            if (aguardandoEscolhaCor) {
                // QUEM JOGOU O INCOLOR escolhe a cor
                if (IsKeyPressed(KEY_ONE)) {
                    game.corAtual = VERMELHO;
                    aguardandoEscolhaCor = 0;
                    next_player();  // Agora sim passa o turno
                }
                else if (IsKeyPressed(KEY_TWO)) {
                    game.corAtual = AMARELO;
                    aguardandoEscolhaCor = 0;
                    next_player();
                }
                else if (IsKeyPressed(KEY_THREE)) {
                    game.corAtual = VERDE;
                    aguardandoEscolhaCor = 0;
                    next_player();
                }
                else if (IsKeyPressed(KEY_FOUR)) {
                    game.corAtual = AZUL;
                    aguardandoEscolhaCor = 0;
                    next_player();
                }
            }
            else {
                processarTurnoJogador(cartasUI, numCartasUI, &assets);

                // Verificar se acabou de jogar INCOLOR (antes de passar o turno)
                Carta* topo = check_top(game.pilha);
                if (topo != NULL && topo->cor == INCOLOR) {
                    aguardandoEscolhaCor = 1;
                }
            }
        }
        else {
            // Turno da CPU
            processarTurnoCPU();
            WaitTime(0.5);  // Pausa para visualiza��o
        }

        // Desenhar
        desenharTela(&assets, cartasUI, numCartasUI, background, jogadorHumano);

        // Mostrar escolha de cor
        if (aguardandoEscolhaCor) {
            BeginDrawing();
            DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.5f));
            DrawText("VOCE jogou um INCOLOR!", LARGURA_TELA / 2 - 200, ALTURA_TELA / 2 - 140, 28, GOLD);
            DrawText("VOCE escolhe a cor:", LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 - 100, 30, WHITE);
            DrawText("1 - Vermelho", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 - 50, 25, RED);
            DrawText("2 - Amarelo", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 - 20, 25, YELLOW);
            DrawText("3 - Verde", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 + 10, 25, GREEN);
            DrawText("4 - Azul", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 + 40, 25, BLUE);
            EndDrawing();
        }
    }

    // Limpeza
    UnloadTexture(background);
    descarregarAssets(&assets);
    liberarJogo();
    CloseWindow();
    
    return 0;
}

// Processa o turno do jogador humano
void processarTurnoJogador(CartaUI* cartasUI, int numCartasUI, Assets* assets) {
    Vector2 mousePos = GetMousePosition();

    // Verificar se tem que comprar cartas obrigat�rias (precisa clicar no baralho)
    if (game.comprar_cartas > 0) {
        Rectangle baralhoRect = { LARGURA_TELA / 2 + 60, ALTURA_TELA / 2 - ALTURA_CARTA / 2,
                                 LARGURA_CARTA, ALTURA_CARTA };

        if (CheckCollisionPointRec(mousePos, baralhoRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < game.comprar_cartas; i++) {
                puxar_baralho(game.jogador_da_vez, game.baralho);
            }
            game.comprar_cartas = 0;
            next_player();
        }
        return;
    }

    // Verificar se est� bloqueado (passa automaticamente ap�s 1 segundo)
    static double tempoBloqueio = 0;
    if (game.jogador_bloqueado) {
        if (tempoBloqueio == 0) {
            tempoBloqueio = GetTime();
        }
        if (GetTime() - tempoBloqueio > 1.0) {
            game.jogador_bloqueado = 0;
            tempoBloqueio = 0;
            next_player();
        }
        return;
    }
    tempoBloqueio = 0;

    // Clique no baralho para comprar (N�O passa a vez automaticamente)
    Rectangle baralhoRect = { LARGURA_TELA / 2 + 60, ALTURA_TELA / 2 - ALTURA_CARTA / 2,
                             LARGURA_CARTA, ALTURA_CARTA };

    if (CheckCollisionPointRec(mousePos, baralhoRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        (game, game.jogador_da_vez);
        // Jogador pode continuar tentando jogar ap�s comprar
        return;
    }

    // Clique em uma carta
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < numCartasUI; i++) {
            if (CheckCollisionPointRec(mousePos, cartasUI[i].bounds)) {
                Carta* cartaSelecionada = cartasUI[i].carta;
                Carta* cartaTopo = check_top(game.pilha);

                if (cartaTopo != NULL && validar_jogada(cartaSelecionada, game.pilha)) {
                    // Remover carta da m�o
                    Carta* carta = jogar_pilha(game.jogador_da_vez, count(cartaSelecionada));

                    if (carta != NULL) {
                        jogada(game.jogador_da_vez, carta);
                        free(carta);

                        // Sempre passa a vez ap�s jogar uma carta
                        // (a menos que seja INCOLOR, que precisa escolher cor primeiro)
                        if (cartaSelecionada->cor != INCOLOR) {
                            next_player();
                        }
                    }
                }
                break;
            }
        }
    }

    // Tecla SPACE para passar a vez (ap�s comprar ou quando n�o tem jogada)
    if (IsKeyPressed(KEY_SPACE)) {
        next_player();
    }
}

// Processa o turno da CPU
void processarTurnoCPU() {
    // Verificar se tem que comprar cartas obrigat�rias
    if (game.comprar_cartas > 0) {
        for (int i = 0; i < game.comprar_cartas; i++) {
            puxar_baralho(&game.jogador_da_vez, &game.baralho);
        }
        game.comprar_cartas = 0;
        next_player();
        return;
    }

    // Verificar se est� bloqueado
    if (game.jogador_bloqueado) {
        game.jogador_bloqueado = 0;
        next_player();
        return;
    }

    // Procurar carta jog�vel
    Carta* cartaTopo = check_top(game.pilha);
    CartaNode* cartaAtual = game.jogador_da_vez->mao;

    int jogadaBot = decidirJogadaBot(game.jogador_da_vez->mao);

    if (jogadaBot <= 0) { // O gemini não escolheu nehuma carta
        puxar_baralho(&game.jogador_da_vez, &game.baralho);
        next_player();
        return;    
    }

    if (jogadaBot >= 1) {
        // Jogar carta
        Carta* carta = jogar_pilha(game.jogador_da_vez, jogadaBot);

    if (carta != NULL) {
        aplicar_efeito_carta(*game.jogador_da_vez, carta); //ljnnkjbhbkk
        
    if (carta->cor == INCOLOR) {
        game.corAtual = escolherCorINCOLOR(0);
    }
}
    next_player();
    }
}

// Verifica se algu�m venceu
void verificarVitoria() {
    PlayerNode* temp = game.jogador_da_vez;

    do {
        if (count(temp) == 0) {
            game.jogoTerminado = 1;
            game.vencedor = temp;
            return;
        }
        temp = temp->next;
    } while (temp != game.jogador_da_vez);
}
