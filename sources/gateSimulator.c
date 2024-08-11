#include <raylib.h>
#include <rlgl.h>
#include "editor.h"

// ----------------------------------------------------------------------------
// Window information
// ----------------------------------------------------------------------------

const size_t WINDOW_WIDTH = 600;
const size_t WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "Gate Simulator";
const size_t MAX_FPS = 144;

// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------

void Init(void);
void Update(void);
void Draw(void);

// ----------------------------------------------------------------------------
// Entry point
// ----------------------------------------------------------------------------
int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(MAX_FPS);
    //SetExitKey(KEY_NULL);
    
    Init();

    // Main Loop --------------------------------------------------------------
    while(!WindowShouldClose()) {
        Update();
        Draw();
    }
    // ------------------------------------------------------------------------

    // De-Initialization ------------------------------------------------------
    CloseWindow();
    // ------------------------------------------------------------------------

    return 0;
}

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------
void Init(void) {
    InitEntities();
    InitGateTypes();
    InitEditor();
}

// ----------------------------------------------------------------------------
// Update
// ----------------------------------------------------------------------------
void Update(void) {
    UpdateEditor();
}

// ----------------------------------------------------------------------------
// Draw next frame
// ----------------------------------------------------------------------------
void Draw(void) {
    BeginDrawing();

        const Color guiBGColor = {0x5c, 0x57, 0x57, 0xff};
        ClearBackground(guiBGColor);

        DrawEditor();

        DrawFPS(0, 0);

    EndDrawing();
}