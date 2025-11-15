
#include "scene_manager.h"

typedef enum {
    NONE,
    RUNNING,
    LOADING,
} SceneState;

static Scene current_scene;
static SceneState scene_state;

void InitSceneManager(void) {
    scene_state = RUNNING;
}

void SceneLoop(void) {
    scene_state = RUNNING;
    current_scene.update_scene();

    BeginDrawing();
    ClearBackground(WHITE);
    current_scene.draw_scene();
    EndDrawing();
}

void ChangeScene(Scene new_scene) {
    scene_state = LOADING;
    if (current_scene.unload_scene)
        current_scene.unload_scene();
    current_scene = new_scene;
    current_scene.load_scene();
}

void EndSceneManager(void) {
    current_scene.unload_scene();
    scene_state = NONE;
}

bool GameShouldQuit(void) {
    return scene_state == NONE;
}

