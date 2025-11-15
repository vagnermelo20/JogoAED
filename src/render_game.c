#include "raylib.h"
#include "render_game.h"
#include "game_manager.h"
#include <stdio.h>
#include <string.h>

// Carrega todos os assets
Assets carregarAssets() {
    Assets assets;

    // Carregar cartas numéricas
    const char* coresNomes[] = { "red", "yellow", "green", "blue" };

    for (int cor = 0; cor < 4; cor++) {
        for (int num = 0; num < 10; num++) {
            char caminho[100];
            sprintf(caminho, "assets/%s_%d.png", coresNomes[cor], num);

            Image img = LoadImage(caminho);
            ImageResize(&img, LARGURA_CARTA, ALTURA_CARTA);
            assets.cartasNumericas[cor][num] = LoadTextureFromImage(img);
            UnloadImage(img);
        }
    }

    // Carregar cartas especiais coloridas
    const char* especiaisNomes[] = { "draw_two", "reverse", "skip" };

    for (int cor = 0; cor < 4; cor++) {
        for (int tipo = 0; tipo < 3; tipo++) {
            char caminho[100];
            sprintf(caminho, "assets/%s_%s.png", coresNomes[cor], especiaisNomes[tipo]);

            Image img = LoadImage(caminho);
            ImageResize(&img, LARGURA_CARTA, ALTURA_CARTA);
            assets.cartasEspeciais[cor][tipo] = LoadTextureFromImage(img);
            UnloadImage(img);
        }
    }

    // Carregar INCOLOR
    Image imgWild = LoadImage("assets/wild.png");
    ImageResize(&imgWild, LARGURA_CARTA, ALTURA_CARTA);
    assets.cartasIncolor = LoadTextureFromImage(imgWild);
    UnloadImage(imgWild);

    // Carregar INCOLOR +4
    Image imgWildFour = LoadImage("assets/wild_draw_four.png");
    ImageResize(&imgWildFour, LARGURA_CARTA, ALTURA_CARTA);
    assets.cartaIncolorMaisQuatro = LoadTextureFromImage(imgWildFour);
    UnloadImage(imgWildFour);

    // Carregar verso da carta
    Image imgBack = LoadImage("assets/card_back.png");
    ImageResize(&imgBack, LARGURA_CARTA, ALTURA_CARTA);
    assets.cartaVerso = LoadTextureFromImage(imgBack);
    UnloadImage(imgBack);

    // Carregar fonte
    assets.fonte = GetFontDefault();

    return assets;
}

// Descarrega assets
void descarregarAssets(Assets* assets) {
    for (int cor = 0; cor < 4; cor++) {
        for (int num = 0; num < 10; num++) {
            UnloadTexture(assets->cartasNumericas[cor][num]);
        }
        for (int tipo = 0; tipo < 3; tipo++) {
            UnloadTexture(assets->cartasEspeciais[cor][tipo]);
        }
    }
    UnloadTexture(assets->cartasIncolor);
    UnloadTexture(assets->cartaIncolorMaisQuatro);
    UnloadTexture(assets->cartaVerso);
}

// Desenha uma carta
void desenharCarta(Carta* carta, Vector2 posicao, Assets* assets, int mostrarFrente) {
    if (!mostrarFrente) {
        DrawTextureV(assets->cartaVerso, posicao, WHITE);
        return;
    }

    if (carta->cor == INCOLOR) {
        // INCOLOR +4
        if (carta->valor == MAIS_4) {
            DrawTextureV(assets->cartaIncolorMaisQuatro, posicao, WHITE);
        }
        else {
            // INCOLOR normal
            DrawTextureV(assets->cartasIncolor, posicao, WHITE);
        }
    }
    else if (carta->valor <= NOVE) {
        // Carta numérica
        DrawTextureV(assets->cartasNumericas[carta->cor][carta->valor], posicao, WHITE);
    }
    else {
        // Cartas especiais coloridas
        int tipoEspecial = -1;

        if (carta->valor == MAIS_2) {
            tipoEspecial = 0; // draw_two
        }
        else if (carta->valor == REVERSO) {
            tipoEspecial = 1; // reverse
        }
        else if (carta->valor == BLOQUEAR) {
            tipoEspecial = 2; // skip
        }

        if (tipoEspecial >= 0) {
            DrawTextureV(assets->cartasEspeciais[carta->cor][tipoEspecial], posicao, WHITE);
        }
    }

    // Desenhar borda se necessário
    DrawRectangleLinesEx((Rectangle) { posicao.x, posicao.y, LARGURA_CARTA, ALTURA_CARTA }, 2, DARKGRAY);
}

// Desenha a mão do jogador
void desenharMaoJogador(CartaNode* mao, Assets* assets, CartaUI* cartasUI, int* numCartas) {
    *numCartas = 0;
    Carta* carta = mao->carta;

    float espacamento = 15;
    float totalLargura = count(mao) * (LARGURA_CARTA + espacamento);
    float startX = (LARGURA_TELA - totalLargura) / 2;
    float startY = ALTURA_TELA - ALTURA_CARTA - 20;

    int i = 0;
    while (carta != NULL && i < 20) {  // Limite de 20 cartas visíveis
        Vector2 pos = { startX + i * (LARGURA_CARTA + espacamento), startY };

        if (cartasUI != NULL) {
            cartasUI[i].bounds = (Rectangle){ pos.x, pos.y, LARGURA_CARTA, ALTURA_CARTA };
            cartasUI[i].carta = carta;
            cartasUI[i].selecionada = 0;
        }

        desenharCarta(carta, pos, assets, 1);

        mao = mao->next;
        i++;
    }

    *numCartas = i;
}

// Desenha a pilha de jogo
void desenharPilhaJogo(Pilha** pile, Assets* assets, Cor corAtual) {
    Vector2 pos = { LARGURA_TELA / 2 - LARGURA_CARTA - 60, ALTURA_TELA / 2 - ALTURA_CARTA / 2 };

    if ((*pile)->carta != NULL) {
        Carta* cartaModificada = (*pile)->carta;

        // Se for INCOLOR, usar a cor atual
        if (cartaModificada->cor == INCOLOR && cartaModificada->valor != MAIS_4) {
            cartaModificada->cor = corAtual;
        }

        desenharCarta(&cartaModificada, pos, assets, 1);
    }
    else {
        DrawRectangle(pos.x, pos.y, LARGURA_CARTA, ALTURA_CARTA, LIGHTGRAY);
        DrawText("Pilha\nVazia", pos.x + 15, pos.y + 50, 20, DARKGRAY);
    }

    // Desenhar indicador de cor atual se for INCOLOR
    if ((*pile)->carta != NULL && (*pile)->carta->cor == INCOLOR) {
        Color corDisplay;

        switch (corAtual) {
        case VERMELHO: corDisplay = RED; break;
        case AMARELO: corDisplay = YELLOW; break;
        case VERDE: corDisplay = GREEN; break;
        case AZUL: corDisplay = BLUE; break;
        default: corDisplay = GRAY; break;
        }

        DrawRectangle(pos.x + LARGURA_CARTA + 10, pos.y + 50, 40, 40, corDisplay);
        DrawRectangleLinesEx((Rectangle) { pos.x + LARGURA_CARTA + 10, pos.y + 50, 40, 40 }, 2, BLACK);
    }
}

// Desenha o baralho
void desenharBaralho(Pilha* deck, Assets* assets, Vector2 posicao) {
    // Desenhar várias cartas sobrepostas para efeito de pilha
    for (int i = 0; i < 3; i++) {
        Vector2 offset = { posicao.x + i * 2, posicao.y - i * 2 };
        DrawTextureV(assets->cartaVerso, offset, WHITE);
    }

    // Desenhar contador
    char texto[20];
    sprintf(texto, "%d", count(deck));
    DrawText(texto, posicao.x + 35, posicao.y + ALTURA_CARTA + 5, 20, BLACK);
}

// Desenha informações do jogo
void desenharInfoJogo( Assets* assets) {
    // Título
    DrawText("UNO - Jogo", 20, 20, 30, DARKBLUE);

    // Jogador atual com destaque
    char textoJogador[100];
    sprintf(textoJogador, "Turno: %s", game.jogador_da_vez);

    Color corTurno = (strcmp(game.jogador_da_vez, "Você") == 0) ? GREEN : ORANGE;
    DrawRectangle(15, 55, 250, 35, Fade(corTurno, 0.3f));
    DrawText(textoJogador, 20, 60, 25, BLACK);

    // Direção
    const char* direcao = (game.direcao == ANTI_CLOCKWISE) ? "Sentido: >" : "Sentido: <";
    DrawText(direcao, 20, 95, 20, WHITE);

    // Cartas para comprar
    if (game.comprar_cartas > 0) {
        char textoCompra[50];
        sprintf(textoCompra, "COMPRE %d CARTAS!", game.comprar_cartas);
        DrawRectangle(LARGURA_TELA / 2 - 150, 80, 300, 50, Fade(RED, 0.8f));
        DrawText(textoCompra, LARGURA_TELA / 2 - 130, 95, 25, WHITE);
    }

    // Outros jogadores
    int y = 150;
    PlayerNode* temp = game.jogador_da_vez->next;
    while (temp != game.jogador_da_vez) {
        char info[100];
        sprintf(info, "%s: %d cartas", temp, count(temp));
        DrawText(info, LARGURA_TELA - 200, y, 18, WHITE);
        y += 25;
        temp = temp->next;
    }

    // Instruções
    DrawText("Clique em uma carta para jogar", 20, ALTURA_TELA - 200, 18, WHITE);
    DrawText("Clique no baralho para comprar", 20, ALTURA_TELA - 175, 18, WHITE);
    DrawText("Pressione ESPACO para passar a vez", 20, ALTURA_TELA - 150, 18, WHITE);

    // Jogador bloqueado
    if (game.jogador_bloqueado) {
        DrawText("BLOQUEADO! Aguarde...", LARGURA_TELA / 2 - 120, 150, 25, RED);
    }
}

// Desenha a tela completa
void desenharTela(Assets* assets, CartaUI* cartasJogador, int numCartasJogador, Texture2D background, Player* jogadorHumano) {
    BeginDrawing();

    // Desenhar background
    DrawTexture(background, 0, 0, WHITE);

    // Informações do jogo
    desenharInfoJogo(assets);

    // Pilha de jogo (centro)
    desenharPilhaJogo(game.pilha, assets, game.corAtual);

    // Baralho (ao lado da pilha)
    Vector2 posBaralho = { LARGURA_TELA / 2 + 60, ALTURA_TELA / 2 - ALTURA_CARTA / 2 };
    desenharBaralho(game.baralho, assets, posBaralho);
    DrawText("Comprar", posBaralho.x + 10, posBaralho.y - 25, 18, BLACK);

    // Mão do jogador HUMANO (sempre mostra suas cartas)
    int numCartas;
    desenharMaoJogador(jogadorHumano, assets, cartasJogador, &numCartas);

    // Mensagem de vitória
    if (game.jogoTerminado && game.vencedor != NULL) {
        DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, Fade(BLACK, 0.7f));

        char mensagem[100];
        sprintf(mensagem, "%s VENCEU!", game.vencedor->nome);
        int larguraTexto = MeasureText(mensagem, 50);
        DrawText(mensagem, LARGURA_TELA / 2 - larguraTexto / 2, ALTURA_TELA / 2 - 50, 50, GOLD);

        DrawText("Pressione ESC para sair", LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 + 20, 25, WHITE);
    }

    EndDrawing();
}

// Obtém nome da cor
const char* obterNomeCor(Cor cor) {
    switch (cor) {
    case INCOLOR: return "Incolor";
    case VERMELHO: return "Vermelho";
    case AMARELO: return "Amarelo";
    case VERDE: return "Verde";
    default: return "Desconhecida";
    }
}

// Obtém nome do valor
const char* obterNomeValor(Valor valor) {
    switch (valor) {
    case ZERO: return "0";
    case UM: return "1";
    case DOIS: return "2";
    case TRES: return "3";
    case QUATRO: return "4";
    case CINCO: return "5";
    case SEIS: return "6";
    case SETE: return "7";
    case OITO: return "8";
    case NOVE: return "9";
    case BLOQUEAR: return "Bloquear";
    case BLOQUEAR_ANTERIOR: return "Bloquear anterior";
    case MAIS_2: return "+2";
    case MAIS_4: return "+4";
    case REVERSO: return "Reverso";
    case DEFINIR_COR: return "Cor";
    default: return "?";
    }
}
