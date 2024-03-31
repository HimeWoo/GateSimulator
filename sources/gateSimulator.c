#include <raylib.h>
#include <rlgl.h>
#include "baseObject.h"
#include "himegui.h"

// Constants ------------------------------------------------------------------
const size_t DEFAULT_WINDOW_WIDTH = 600;
const size_t DEFAULT_WINDOW_HEIGHT = 480;
const char* DEFAULT_WINDOW_TITLE = "gateSimulator";
const size_t DEFAULT_MAX_FPS = 144;
// ----------------------------------------------------------------------------

int main(void) {
    // Initialization ---------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                DEFAULT_WINDOW_TITLE);
    SetTargetFPS(DEFAULT_MAX_FPS);
    // SetExitKey(KEY_NULL);
    
    //Texture2D texture = LoadTexture(ASSETS_PATH "testimage.png");
    
    /* BaseObject testObj = CLITERAL(BaseObject){
        CLITERAL(Vector2) {0, 0},
        texture,
        0,
        0.1f
    }; */
    
    const float toolBorderThickness = 2.0f;
    const Color toolBorderColor = WHITE;
    
    Rectangle toolPanel = {
        0,
        0,
        GetScreenWidth()/4,
        GetScreenHeight()
    };
    Rectangle editorPanel = {
        toolPanel.width,
        0,
        GetScreenWidth() - toolPanel.width,
        GetScreenHeight()
    };
    Rectangle testButton = {
        toolPanel.x + toolBorderThickness,
        toolPanel.y + toolBorderThickness,
        toolPanel.width - 2 * toolBorderThickness,
        50
    };
    RenderTexture2D gridLines = LoadRenderTexture(
        editorPanel.width,
        editorPanel.height
    );
    Camera2D editorCam = {
        CLITERAL(Vector2) {
            editorPanel.width/2,
            editorPanel.height/2
        },
        CLITERAL(Vector2) {0, 0},
        0,
        1
    };

    // ------------------------------------------------------------------------
    
    // Main Loop --------------------------------------------------------------
    while(!WindowShouldClose()) {
        // Mouse information ----------
        Vector2 mousePos = GetMousePosition();
        Vector2 mouseDelta = GetMouseDelta();
        // ----------------------------
        if (IsWindowResized()) {
            toolPanel.width = GetScreenWidth()/4;
            toolPanel.height = GetScreenHeight();
            editorPanel.x = toolPanel.width,
            editorPanel.width = GetScreenWidth() - toolPanel.width;
            editorPanel.height = GetScreenHeight();
            gridLines = LoadRenderTexture(
                editorPanel.width,
                editorPanel.height
            );
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = Vector2Scale(mouseDelta, -1.0f / editorCam.zoom);
            editorCam.target = Vector2Add(editorCam.target, delta);
        }
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            // Mouse position wrt the editor
            Vector2 mouseEditorPos = CLITERAL(Vector2) {
                mousePos.x - editorPanel.x,
                mousePos.y - editorPanel.y
            };
            Vector2 mouseWorldPos = GetScreenToWorld2D(mouseEditorPos, editorCam);
            editorCam.offset = mouseEditorPos;
            editorCam.target = mouseWorldPos;
            const float ZOOM_INCREMENT = 0.05f;
            const float MIN_ZOOM = 0.1f;
            const float MAX_ZOOM = 2.0f;
            float targetZoom = editorCam.zoom + wheel * ZOOM_INCREMENT;
            editorCam.zoom = Clamp(targetZoom, MIN_ZOOM, MAX_ZOOM);
        }

        BeginDrawing();

            const Color guiBGColor = {0x5c, 0x57, 0x57, 0xff};
            ClearBackground(guiBGColor);
            DrawGrid2D(editorPanel, editorCam, gridLines);
            if (CheckCollisionPointRec(mousePos, testButton)) {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    DrawRectangleRec(testButton, DARKGRAY);
                } else {
                    DrawRectangleRec(testButton, GRAY);
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    printf("Button clicked\n");
                }
            } else {
                DrawRectangleRec(testButton, WHITE);
            }
            DrawRectangleLinesEx(testButton, 4.0, BLACK);
            DrawRectangleLinesEx(toolPanel, toolBorderThickness, toolBorderColor);
            DrawRectangleLinesEx(editorPanel, toolBorderThickness, toolBorderColor);

            BeginMode2D(editorCam);


            
            EndMode2D();

            DrawFPS(0, 0);

        EndDrawing();
    }
    // ------------------------------------------------------------------------

    // De-Initialization ------------------------------------------------------
    UnloadRenderTexture(gridLines);

    CloseWindow();
    // ------------------------------------------------------------------------

    return 0;
}