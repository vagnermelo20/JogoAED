#include "raylib.h"
#include "render_game.h"
#include "game_manager.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>

// Carrega todos os assets
Assets carregarAssets() {
    Assets assets;

    // Carregar cartas numéricas - AJUSTADO PARA 4 CORES
    const char* coresNomes[] = {"yellow", "blue", "green", "red"}; // Índices 0-3

    for (int cor = 0; cor < 4; cor++) { // APENAS AS 4 CORES NORMAIS
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

    for (int cor = 0; cor < 4; cor++) { // APENAS AS 4 CORES NORMAIS
        for (int tipo = 0; tipo < 3; tipo++) {
            char caminho[100];
            sprintf(caminho, "assets/%s_%s.png", coresNomes[cor], especiaisNomes[tipo]);

            Image img = LoadImage(caminho);
            ImageResize(&img, LARGURA_CARTA, ALTURA_CARTA);
            assets.cartasEspeciais[cor][tipo] = LoadTextureFromImage(img);
            UnloadImage(img);
        }
    }

    // Carregar INCOLOR (ÍNDICE 4 - separado)
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

// Desenha a mao do jogador
void desenharMaoJogador(CartaNode* mao, Assets* assets, CartaUI* cartasUI, int* numCartas) {
    *numCartas = 0;
    if (mao == NULL) return; // Proteção se a mão estiver vazia

    // Assumindo que 'contar_mao' é a função correta
    float espacamento = 15;
    float totalLargura = contar_mao(mao) * (LARGURA_CARTA + espacamento);
    float startX = (LARGURA_TELA - totalLargura) / 2;
    float startY = ALTURA_TELA - ALTURA_CARTA - 20;

    int i = 0;
    CartaNode* noAtual = mao; // Iterar com o nó
    while (noAtual != NULL && i < 20) {  // Limite de 20 cartas visíveis
        Carta* carta = noAtual->carta; // Pegar a carta do nó
        Vector2 pos = { startX + i * (LARGURA_CARTA + espacamento), startY };

        if (cartasUI != NULL) {
            cartasUI[i].bounds = (Rectangle){ pos.x, pos.y, LARGURA_CARTA, ALTURA_CARTA };
            cartasUI[i].carta = carta;
            cartasUI[i].selecionada = 0;
        }

        desenharCarta(carta, pos, assets, 1);

        noAtual = noAtual->next;
        i++;
    }

    *numCartas = i;
}

// Desenha a pilha de jogo
void desenharPilhaJogo(Pilha** pile, Assets* assets, Cor corAtual) {
    Vector2 pos = { LARGURA_TELA / 2 - LARGURA_CARTA - 60, ALTURA_TELA / 2 - ALTURA_CARTA / 2 };

    // Proteção contra ponteiro nulo e pilha vazia
    if (pile == NULL || *pile == NULL || (*pile)->carta == NULL) {
        DrawRectangle(pos.x, pos.y, LARGURA_CARTA, ALTURA_CARTA, LIGHTGRAY);
        DrawText("Pilha\nVazia", pos.x + 15, pos.y + 50, 20, DARKGRAY);
        return;
    }


    desenharCarta((*pile)->carta, pos, assets, 1);


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
    sprintf(textoJogador, "Turno: p%d", game.jogador_da_vez->numero);
    DrawText(textoJogador, 20, 60, 25, BLACK);

    // Direção
    const char* direcao = (game.direcao == ANTI_CLOCKWISE) ? "Sentido: >" : "Sentido: <";
    DrawText(direcao, 20, 95, 20, WHITE);
    // Outros jogadores
    int y = 150;
    if (game.jogador_da_vez != NULL) {
        PlayerNode* temp = game.jogador_da_vez->next;

        // 2. O loop deve parar se a lista for quebrada (temp == NULL) ou se der a volta
        while (temp != NULL && temp != game.jogador_da_vez) {
            char info[100];

            int numCartas = contar_mao(temp->mao);
            sprintf(info, "p%d: %d cartas", temp->numero, numCartas);

            DrawText(info, LARGURA_TELA - 200, y, 18, WHITE);
            y += 25;
            temp = temp->next;
        }
    }

    // Instruções
    DrawText("Clique em uma carta para jogar", 20, ALTURA_TELA - 225, 18, WHITE);
    DrawText("Clique no baralho ou presione espaço para comprar", 20, ALTURA_TELA - 200, 18, WHITE);

}

// Desenha a tela completa
void desenharTela(Assets* assets, CartaUI* cartasJogador, int numCartasJogador, Texture2D background, Player* jogadorHumano) {
    BeginDrawing();

    // Desenhar background
    DrawTexture(background, 0, 0, WHITE);

    // Informações do jogo
    desenharInfoJogo(assets);

    // Pilha de jogo (centro)
    desenharPilhaJogo(&game.pilha, assets, game.corAtual);

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
        sprintf(mensagem, "p%d VENCEU!", game.vencedor->numero);
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
    case AMARELO: return "Amarelo";
    case AZUL: return "Azul";
    case VERDE: return "Verde";
    case VERMELHO: return "Vermelho";
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
    case MAIS_2: return "+2";
    case MAIS_4: return "+4";
    case DEFINIR_COR: return "Cor";
    case BLOQUEAR: return "Bloquear";
    case REVERSO: return "Reverso";
    default: return "?";
    }
}
