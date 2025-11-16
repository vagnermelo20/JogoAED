#include "tela_ganhou.h"
#include <string.h>
#include <stdio.h>

void DrawTelaGanhou(Assets* recursos, Texture2D texturaFundo, const char* nomeVencedor, int* jogarNovamente) {
    Vector2 posicaoMouse = GetMousePosition();

    BeginDrawing();
    // Desenha a mesma textura de fundo usada pelo jogo para consistência
    DrawTexture(texturaFundo, 0, 0, WHITE);

    // Mensagem do vencedor
    char mensagemVencedor[256];
    if (nomeVencedor != NULL) snprintf(mensagemVencedor, sizeof(mensagemVencedor), "%s VENCEU!", nomeVencedor);
    else snprintf(mensagemVencedor, sizeof(mensagemVencedor), "VENCEDOR!");

    int tamanhoFonte = 48;
    int larguraTexto = MeasureText(mensagemVencedor, tamanhoFonte);
    DrawText(mensagemVencedor, LARGURA_TELA/2 - larguraTexto/2, ALTURA_TELA/3 - 40, tamanhoFonte, GOLD);

    // Botão "Jogar novamente"
    const char* rotuloBotao = "Jogar novamente";
    int larguraBotao = 300;
    int alturaBotao = 60;
    Rectangle botaoRect = {
        (LARGURA_TELA - larguraBotao) / 2.0f,
        (ALTURA_TELA) / 2.0f,
        (float)larguraBotao,
        (float)alturaBotao
    };

    Color corBotao = CheckCollisionPointRec(posicaoMouse, botaoRect) ? Fade(LIGHTGRAY, 0.9f) : LIGHTGRAY;
    DrawRectangleRec(botaoRect, corBotao);

    int larguraRotulo = MeasureText(rotuloBotao, 26);
    DrawText(rotuloBotao,
             (int)(botaoRect.x + (botaoRect.width - larguraRotulo) / 2),
             (int)(botaoRect.y + (botaoRect.height - 26) / 2),
             26, BLACK);

    // Clique no botão
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(posicaoMouse, botaoRect)) {
        if (jogarNovamente) *jogarNovamente = 1;
    }

    EndDrawing();
}