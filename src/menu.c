#include "menu.h"
#include "raylib.h"
#include <stdio.h>

MenuState DrawMenu(Texture2D background) {
    Vector2 mousePos = GetMousePosition();
    MenuState nextState = MENU_PRINCIPAL;
    
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    // Desenhar fundo
    DrawTexture(background, 0, 0, WHITE);
    
    // Desenhar título com sombra
    const char* title = "UNO";
    int titleSize = 120;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = GetScreenWidth() / 2 - titleWidth / 2;
    int titleY = 100;
    
    DrawText(title, titleX + 4, titleY + 4, titleSize, Fade(BLACK, 0.6f)); // Sombra
    DrawText(title, titleX, titleY, titleSize, ORANGE);
    
    // Subtítulo
    const char* subtitle = "Jogo de Cartas";
    int subtitleWidth = MeasureText(subtitle, 30);
    DrawText(subtitle, GetScreenWidth() / 2 - subtitleWidth / 2, titleY + titleSize + 10, 30, LIGHTGRAY);
    
    // Botão Jogar
    int buttonWidth = 300;
    int buttonHeight = 70;
    int centerX = GetScreenWidth() / 2 - buttonWidth / 2;
    int startY = GetScreenHeight() / 2 - 50;
    
    Rectangle jogarButton = {centerX, startY, buttonWidth, buttonHeight};
    bool hoveringJogar = CheckCollisionPointRec(mousePos, jogarButton);
    Color jogarColor = hoveringJogar ? Fade(GOLD, 0.95f) : GOLD;
    
    DrawRectangleRec(jogarButton, jogarColor);
    DrawRectangleLinesEx(jogarButton, 4, BLACK);
    
    const char* jogarText = "JOGAR";
    int jogarTextWidth = MeasureText(jogarText, 35);
    DrawText(jogarText, jogarButton.x + (jogarButton.width - jogarTextWidth) / 2,
             jogarButton.y + (jogarButton.height - 35) / 2, 35, BLACK);
    
    if (hoveringJogar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        nextState = MENU_JOGAR;
    }
    
    // Botão Instruções
    Rectangle instrucoesButton = {centerX, startY + buttonHeight + 30, buttonWidth, buttonHeight};
    bool hoveringInstrucoes = CheckCollisionPointRec(mousePos, instrucoesButton);
    Color instrucoesColor = hoveringInstrucoes ? Fade(SKYBLUE, 0.95f) : SKYBLUE;
    
    DrawRectangleRec(instrucoesButton, instrucoesColor);
    DrawRectangleLinesEx(instrucoesButton, 4, BLACK);
    
    const char* instrucoesText = "INSTRUÇÕES";
    int instrucoesTextWidth = MeasureText(instrucoesText, 35);
    DrawText(instrucoesText, instrucoesButton.x + (instrucoesButton.width - instrucoesTextWidth) / 2,
             instrucoesButton.y + (instrucoesButton.height - 35) / 2, 35, BLACK);
    
    if (hoveringInstrucoes && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        nextState = MENU_INSTRUCOES;
    }
    
    // Rodapé
    const char* footer = "Pressione ESC para sair";
    int footerWidth = MeasureText(footer, 18);
    DrawText(footer, GetScreenWidth() / 2 - footerWidth / 2, GetScreenHeight() - 40, 18, DARKGRAY);
    
    EndDrawing();
    
    return nextState;
}

MenuState DrawInstrucoes(Texture2D background) {
    Vector2 mousePos = GetMousePosition();
    MenuState nextState = MENU_INSTRUCOES;
    
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    // Desenhar fundo
    DrawTexture(background, 0, 0, WHITE);
    
    // Overlay semi-transparente para melhor leitura
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
    
    // Título
    const char* title = "COMO JOGAR";
    int titleSize = 50;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, GetScreenWidth() / 2 - titleWidth / 2, 40, titleSize, GOLD);
    
    // Instruções
    int startY = 80;
    int lineHeight = 35;
    int textSize = 15;
    
    DrawText("OBJETIVO:", 50, startY, 18, ORANGE);
    DrawText("Seja o primeiro jogador a descartar todas as suas cartas!", 50, startY + 30, textSize, WHITE);
    
    startY += 60;
    DrawText("REGRAS BÁSICAS:", 50, startY, 18, ORANGE);
    DrawText("Combine a cor ou o número da carta no topo da pilha", 50, startY + 35, textSize, WHITE);
    DrawText("Cartas especiais têm efeitos únicos", 50, startY + 35 + lineHeight, textSize, WHITE);
    DrawText("Se não puder jogar, compre uma carta do baralho", 50, startY + 35 + lineHeight * 2, textSize, WHITE);
    
    startY += 130;
    DrawText("CARTAS ESPECIAIS:", 50, startY, 18, ORANGE);
    DrawText("+2: Próximo jogador compra 2 cartas", 50, startY + 35, textSize, YELLOW);
    DrawText("+4: Próximo jogador compra 4 cartas + escolhe cor", 50, startY + 35 + lineHeight, textSize, YELLOW);
    DrawText("PULAR: Próximo jogador perde a vez", 50, startY + 35 + lineHeight * 2, textSize, YELLOW);
    DrawText("INVERTER: Inverte o sentido do jogo", 50, startY + 35 + lineHeight * 3, textSize, YELLOW);
    DrawText("CORINGA: Escolha a próxima cor", 50, startY + 35 + lineHeight * 4, textSize, YELLOW);
    
    startY += 195;
    DrawText("CONTROLES:", 50, startY, 18, ORANGE);
    DrawText("Clique nas cartas para jogar", 50, startY + 35, textSize, WHITE);
    DrawText("Clique no baralho (ou ESPAÇO) para comprar", 50, startY + 35 + lineHeight, textSize, WHITE);
    DrawText("Use números 1-4 para escolher cores (cartas coringas)", 50, startY + 35 + lineHeight * 2, textSize, WHITE);

    startY += 140;
    DrawText("CARTAS EXTRAS:", 50, startY, 18, ORANGE);
    DrawText("Descubra", 50, startY + 35, textSize, WHITE);

    // Botão Voltar
    int buttonWidth = 250;
    int buttonHeight = 60;
    Rectangle voltarButton = {
        GetScreenWidth() / 2 - buttonWidth / 2,
        GetScreenHeight() - 100,
        buttonWidth,
        buttonHeight
    };
    
    bool hoveringVoltar = CheckCollisionPointRec(mousePos, voltarButton);
    Color voltarColor = hoveringVoltar ? Fade(RED, 0.95f) : RED;
    
    DrawRectangleRec(voltarButton, voltarColor);
    DrawRectangleLinesEx(voltarButton, 3, BLACK);
    
    const char* voltarText = "VOLTAR";
    int voltarTextWidth = MeasureText(voltarText, 30);
    DrawText(voltarText, voltarButton.x + (voltarButton.width - voltarTextWidth) / 2,
             voltarButton.y + (voltarButton.height - 30) / 2, 30, WHITE);
    
    if (hoveringVoltar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        nextState = MENU_PRINCIPAL;
    }
    
    EndDrawing();
    
    return nextState;
}
