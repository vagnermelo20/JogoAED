#pragma once
#include "game_scene.h"
#include "raylib.h"

typedef enum {
    START,
    QUIT
} Option;

static Option option = START;

static void UpdateOption() {
    game_loop();
}

static void LoadScene(void) {}
static void UnloadScene(void) {}

static void UpdateScene(void) {
    UpdateOption();
}