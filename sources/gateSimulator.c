#include <raylib.h>
#include <rlgl.h>
#include "entity.h"
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

    Texture2D texture = LoadTexture(ASSETS_PATH "testimage.png");

    // Main Loop --------------------------------------------------------------
    while(!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        Vector2 mouseDelta = GetMouseDelta();

        Update();
        Draw();

        /* BeginDrawing();

            const Color guiBGColor = {0x5c, 0x57, 0x57, 0xff};
            ClearBackground(guiBGColor);

            DrawEditor(editorPanel, &editorCam);
            
            if (CheckCollisionPointRec(mousePos, editorPanel)) {
                Vector2 mouseWorldPos = GetScreenToWorld2D(mousePos, editorCam);
                Vector2 gridPos = SnapToGrid(mouseWorldPos, Vector2Zero());
                printf("Grid pos: (%.2f, %.2f)\n", gridPos.x, gridPos.y);
            }

            Color testButtonColor = WHITE;
            if (CheckCollisionPointRec(mousePos, testButton)) {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    testButtonColor = DARKGRAY;
                } else {
                    testButtonColor = GRAY;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    printf("Button clicked\n");
                }
            }
            DrawRectangleRec(testButton, testButtonColor);
            DrawRectangleLinesEx(testButton, 4.0, BLACK);
            
            DrawFPS(0, 0);

        EndDrawing(); */
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