#include "frame.h"
#include "baseObject.h"

// Constants ------------------------------------------------------------------
const size_t DEFAULT_WINDOW_WIDTH = 600;
const size_t DEFAULT_WINDOW_HEIGHT = 480;
const char* DEFAULT_WINDOW_TITLE = "2dgrid";
const size_t DEFAULT_MAX_FPS = 60;
// ----------------------------------------------------------------------------

int main() {
    const Color backgroundColor = {0x5c, 0x57, 0x57, 0xff};
    // Initialization ---------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                DEFAULT_WINDOW_TITLE);
    SetTargetFPS(DEFAULT_MAX_FPS);
    // tests
    Texture2D texture = LoadTexture(ASSETS_PATH "testimage.png");
    Camera2D gridCamera = {(Vector2){0, 0}, (Vector2){0, 0}, 0, 1};
    Frame gridFrame = {300, 0, 300, 240, &gridCamera};
    gridFrame.cam->offset = (Vector2){gridFrame.pos.x + gridFrame.w/2, gridFrame.pos.y + gridFrame.h/2};
    BaseObject testObj = (BaseObject){(Vector2){0, 0}, texture, 0, 1};
    bool isMovingObj = false;
    BaseObject* movingObj;
    const int objCount = 6400;
    BaseObject** objPool = malloc(objCount * sizeof(BaseObject*));
    for (int i = 0; i < objCount; i++) {
        objPool[i] = NewObject( (Vector2){  texture.width * 0.5f * (i % 80),
                                            texture.height * 0.5f * (i / 80)},
                                texture, 0.0f, 0.5f);
    }
    // ------------------------------------------------------------------------
    
    // Main Loop --------------------------------------------------------------
    while(!WindowShouldClose()) {
        // Per-frame information -----
        Vector2 mousePos = GetMousePosition();
        Vector2 mouseWorldPos = GetScreenToWorld2D(mousePos, *gridFrame.cam);
        Vector2 mouseDelta = GetMouseDelta();
        // ---------------------------
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            gridFrame.cam->target = Vector2Add(
                gridFrame.cam->target,
                Vector2Scale(mouseDelta, -1.0f/gridFrame.cam->zoom)
            );
        }

        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            gridFrame.cam->offset = GetMousePosition();
            gridFrame.cam->target = mouseWorldPos;
            const float zoomIncrement = 0.125f;
            const float deltaZoom = wheel * zoomIncrement;
            gridFrame.cam->zoom += deltaZoom;
            if (gridFrame.cam->zoom < zoomIncrement) {
                gridFrame.cam->zoom = zoomIncrement;
            }
        }

        BeginDrawing();
            ClearBackground(backgroundColor);
            
            BeginMode2D(gridCamera);
                for (int i = 0; i < objCount; i++) {
                    if (IsWithinObject(objPool[i], mouseWorldPos)) {
                        DrawObject(objPool[i], BLUE);
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            isMovingObj = true;
                            movingObj = objPool[i];
                        }
                    } else {
                        DrawObject(objPool[i], WHITE);
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                        isMovingObj = false;
                    }
                    if (isMovingObj && (movingObj == objPool[i])) {
                        DrawObject(movingObj, RED);
                        Vector2 translation = Vector2Scale(mouseDelta, 1.0f/gridFrame.cam->zoom);
                        TranslateObject(movingObj, translation);
                    }
                }
                
            EndMode2D();

            DrawFPS(0, 0);
        EndDrawing();

    }
    // ------------------------------------------------------------------------

    CloseWindow();

    return 0;
}