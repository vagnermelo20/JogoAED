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
void processarTurnoJogador(CartaUI* cartasUI, int numCartasUI, Assets* assets);
void processarTurnoCPU();
Cor escolherCorINCOLOR(int ehJogadorHumano);

// --- Mockups para funções de pilha.h (REMOVA QUANDO TIVER AS REAIS) ---
// Estas funções são necessárias para o código compilar,
// mas você deve substituí-las pelas suas implementações reais.

int count_pilha(Pilha* p) {
    int c = 0;
    while (p) { c++; p = p->next; }
    return c;
}
// ---------------------------------------------------------------------


int main() {
    // Inicializar Raylib
    InitWindow(LARGURA_TELA, ALTURA_TELA, "UNO - Jogo de Cartas");
    SetTargetFPS(60);

    // CORREÇÃO: Inicializar Semente Aleatória (necessário para embaralhar)
    srand(time(NULL));

    // Carregar assets
    Assets assets = carregarAssets();

    // --- CORREÇÃO ---
    // Chamar a função de inicialização correta, que cria jogadores
    initialize_game(4);

    // UI
    CartaUI cartasUI[MAX_CARTAS_UI];
    int numCartasUI = 0;
    int aguardandoEscolhaCor = 0;
    Texture2D background = LoadTexture("assets/background_2.png");

    // --- CORREÇÃO ---
    // O tipo é PlayerNode*, não Player*
    // E agora game.jogador_da_vez não é NULL
    PlayerNode* jogadorHumano = game.jogador_da_vez; // Assumir que o P1 é o humano

    // Loop principal
    while (!WindowShouldClose()) {
        // Verificar vitória (usando a função correta)
        verificar_vitoria();

        if (game.jogoTerminado) {
            // Passar a MÃO do jogador humano para desenhar
            desenharTela(&assets, cartasUI, numCartasUI, background, jogadorHumano->mao);
            continue;
        }

        // --- CORREÇÃO ---
        // Passar a MÃO (mao) do jogador, não o jogador (PlayerNode)
        desenharMaoJogador(jogadorHumano->mao, &assets, cartasUI, &numCartasUI);

        // Processar turno
        // --- CORREÇÃO ---
        // Comparar com o nome real "Jogador 1"
        if (game.jogador_da_vez == jogadorHumano) { // Comparação de ponteiros é mais segura
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
                if (topo != NULL && topo->cor == INCOLOR && (topo->valor == DEFINIR_COR  || topo->valor == MAIS_4)){
                    aguardandoEscolhaCor = 1;
                }
            }
        }
        else {
            // Turno da CPU
            processarTurnoCPU();
            WaitTime(0.5);  // Pausa para visualização
        }

        // Desenhar
        // --- CORREÇÃO ---
        // Passar a MÃO do jogador
        desenharTela(&assets, cartasUI, numCartasUI, background, jogadorHumano->mao);

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

    // Verificar se tem que comprar cartas obrigatórias (precisa clicar no baralho)
    if (game.comprar_cartas > 0) {
        Rectangle baralhoRect = { LARGURA_TELA / 2 + 60, ALTURA_TELA / 2 - ALTURA_TELA / 2,
                                 LARGURA_CARTA, ALTURA_CARTA };

        if (CheckCollisionPointRec(mousePos, baralhoRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < game.comprar_cartas; i++) {
                // --- CORREÇÃO ---
                // Passar o endereço do jogador e do baralho
                puxar_baralho(&game.jogador_da_vez, &game.baralho);
            }
            game.comprar_cartas = 0;
            next_player();
        }
        return;
    }

    // Verificar se está bloqueado (passa automaticamente após 1 segundo)
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

    // Clique no baralho para comprar (NÃO passa a vez automaticamente)
    Rectangle baralhoRect = { LARGURA_TELA / 2 + 60, ALTURA_TELA / 2 - ALTURA_CARTA / 2,
                             LARGURA_CARTA, ALTURA_CARTA };

    if (CheckCollisionPointRec(mousePos, baralhoRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // --- CORREÇÃO ---
        // Passar o endereço do jogador e do baralho
        puxar_baralho(&game.jogador_da_vez, &game.baralho);
        next_player(); // comentar essa linha pra permitir puxar mais cartas
        //Jogador pode continuar tentando jogar após comprar == Caetano nunca vai pegar o baralho todo e trocar com alguém
        return;
    }

    // Clique em uma carta
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < numCartasUI; i++) {
            if (CheckCollisionPointRec(mousePos, cartasUI[i].bounds)) {
                Carta* cartaSelecionada = cartasUI[i].carta;

                if (validar_jogada(cartaSelecionada, game.pilha)) { // Assumindo 'validar_jogada'
                    // --- BUG DE LÓGICA CRÍTICO ---
                    // A função 'jogar_pilha' remove a carta DA MÃO e a coloca NA PILHA.
                    // A função 'jogada' está quebrada e não deve ser chamada.
                    // 'jogar_pilha' precisa do ÍNDICE da carta, não de 'count(carta)'.

                    // --- CORREÇÃO (LÓGICA) ---
                    // 'jogar_pilha' espera um índice 1-based
                    Carta* cartaJogada = jogar_pilha(game.jogador_da_vez, i + 1);

                    if (cartaJogada != NULL) {
                        // Aplicar efeitos
                        aplicar_efeito_carta(game.jogador_da_vez, cartaJogada);

                        // Sempre passa a vez após jogar uma carta
                        // (a menos que seja INCOLOR, que precisa escolher cor primeiro)
                        if (cartaJogada->cor != INCOLOR &&  cartaJogada->valor != DEFINIR_COR) {
                            next_player();
                        }
                    }
                }
                break;
            }
        }
    }

    // Tecla SPACE para passar a vez (após comprar ou quando não tem jogada)
    if (IsKeyPressed(KEY_SPACE)) {
        next_player();
    }
}

// Processa o turno da CPU
void processarTurnoCPU() {
    // Verificar se tem que comprar cartas obrigatórias
    if (game.comprar_cartas > 0) {
        for (int i = 0; i < game.comprar_cartas; i++) {
            puxar_baralho(&game.jogador_da_vez, &game.baralho);
        }
        game.comprar_cartas = 0;
        next_player();
        return;
    }

    // Verificar se está bloqueado
    if (game.jogador_bloqueado) {
        game.jogador_bloqueado = 0;
        next_player();
        return;
    }

    // Procurar carta jogável
    CartaNode* cartaAtual = game.jogador_da_vez->mao;
    Carta* melhorCarta = NULL;
    int indiceMelhorCarta = -1;
    int i = 0;

    while (cartaAtual != NULL) {
        if (validar_jogada(cartaAtual->carta, game.pilha)) {
            melhorCarta = cartaAtual->carta;
            indiceMelhorCarta = i;

            // Priorizar cartas especiais
            if (cartaAtual->carta->valor >= MAIS_2) {
                break;
            }
        }
        cartaAtual = cartaAtual->next;
        i++;
    }

    if (melhorCarta != NULL) {
        // Jogar carta
        // --- CORREÇÃO (LÓGICA) ---
        Carta* cartaJogada = jogar_pilha(game.jogador_da_vez, indiceMelhorCarta + 1);

        if (cartaJogada != NULL) {
            // Aplicar efeitos
            aplicar_efeito_carta(game.jogador_da_vez, cartaJogada);

            // Se for INCOLOR, escolher cor
            if (cartaJogada->cor == INCOLOR) {
                game.corAtual = escolherCorINCOLOR(0);
            }
        }
    }
    else {
        // Comprar carta
        puxar_baralho(&game.jogador_da_vez, &game.baralho);
    }

    next_player();
}

// Escolhe cor para INCOLOR (CPU)
Cor escolherCorINCOLOR(int ehJogadorHumano) {
    if (ehJogadorHumano) {
        return VERMELHO;  // Será escolhido pela UI
    }
    
    // CPU: contar cores na mão
    int contadorCores[4] = { 0, 0, 0, 0 };
    CartaNode* carta = game.jogador_da_vez->mao;

    while (carta->next != NULL) {
        if (carta->carta->cor != INCOLOR && carta->carta->cor <= VERMELHO) {
            contadorCores[carta->carta->cor]++;
        }
        carta = carta->next;
    }

    // Escolher cor mais comum
    int maxCor = 0;
    for (int i = 1; i < 4; i++) {
        if (contadorCores[i] > contadorCores[maxCor]) {
            maxCor = i;
        }
    }

    return (Cor)maxCor;
}