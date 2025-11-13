#include "raylib.h"
#include "scene_manager.h"
#include "game_scene.h"

static Scene starting_scene;

void LoadGame(void);
void UnloadGame(void);

//--------------------------------------------------------------
//
//                           MAIN
// 
//--------------------------------------------------------------

int main(void)
{
    InitWindow(1920, 1080, "UNO"); // Inicializa a janela do jogo

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        SceneLoop();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
