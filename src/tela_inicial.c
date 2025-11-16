#include "tela_inicial.h"
#include <string.h>
#include <stdio.h>

void DrawTelaInicial(Assets* assets, Texture2D background, int* startRequested) {
    Vector2 mouse = GetMousePosition();
    BeginDrawing();

    // Draw the same background texture used by the game for consistency
    DrawTexture(background, 0, 0, WHITE);

    // Big title with shadow
    const char* title = "UNO";
    int titleSize = 120;
    int titleW = MeasureText(title, titleSize);
    int titleX = LARGURA_TELA/2 - titleW/2;
    int titleY = ALTURA_TELA/4 - 60;
    DrawText(title, titleX + 4, titleY + 4, titleSize, Fade(BLACK, 0.6f)); // shadow
    DrawText(title, titleX, titleY, titleSize, ORANGE);

    // Subtitle
    const char* subtitle = "Clique em Jogar para começar a partida";
    int subW = MeasureText(subtitle, 22);
    DrawText(subtitle, LARGURA_TELA/2 - subW/2, titleY + titleSize + 10, 22, LIGHTGRAY);

    // Play button (center)
    const char* btnText = "Jogar";
    int btnW = 260, btnH = 70;
    Rectangle btnRect = { (LARGURA_TELA - btnW)/2.0f, (ALTURA_TELA*0.6f) - (btnH/2.0f), (float)btnW, (float)btnH };
    bool hovering = CheckCollisionPointRec(mouse, btnRect);
    Color base = hovering ? Fade(GOLD, 0.95f) : GOLD;
    DrawRectangleRec(btnRect, base);
    DrawRectangleLinesEx(btnRect, 4, BLACK);
    int txtW = MeasureText(btnText, 30);
    DrawText(btnText, (int)(btnRect.x + (btnRect.width - txtW)/2), (int)(btnRect.y + (btnRect.height - 30)/2), 30, BLACK);

    // Click handling
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovering) {
        if (startRequested) *startRequested = 1;
    }


    EndDrawing();
}
