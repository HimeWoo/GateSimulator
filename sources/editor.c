#include "editor.h"
#include "entity.h"

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

const float borderThickness = 2.0f;
const Color borderColor = WHITE;
const int SPACING = 16;

static ClickMode clickMode = CLICK_SELECT;
static ToolMode tool = CREATE_OR;

void InitEditor(void) {
    InitToolbar();
    InitGateTypes();
    editorPanel = CLITERAL(Rectangle) {
        toolPanel.width, 0,
        GetScreenWidth() - toolPanel.width, GetScreenHeight()
    };
    editorCam = CLITERAL(Camera2D) {
        CLITERAL(Vector2) {
            editorPanel.x + editorPanel.width/2,
            editorPanel.y + editorPanel.height/2
        },
        CLITERAL(Vector2) {0, 0},
        0,
        1
    };

    testButton = CLITERAL(Rectangle) {
        toolPanel.x + borderThickness, toolPanel.y + borderThickness,
        toolPanel.width - 2 * borderThickness,
        toolPanel.width - 2 * borderThickness
    };
    testButtonColor = WHITE;
    testTexture = LoadTexture(ASSETS_PATH "testimage.png");
}

Vector2 SnapToGrid(Vector2 pos, Vector2 offset, int tileSize) {
    return CLITERAL(Vector2) {
        //roundf(pos.x / tileSize) * tileSize,
        //roundf(pos.y / tileSize) * tileSize
        floorf(pos.x / tileSize + offset.x) * tileSize,
        floorf(pos.y / tileSize + offset.y) * tileSize
    };
}

void DrawGridLines(Rectangle panel, int tileSize) {
    // Color of the x=0 and y=0 lines
    const Color AXIS_COLOR = CLITERAL(Color) {0x00, 0x40, 0x00, 255};
    // Color of the other grid lines
    const Color LINE_COLOR = CLITERAL(Color) {0x00, 0x20, 0x00, 255};

    // Draw vertical lines
    float minX = tileSize * (floorf(panel.x / tileSize) - 2);
    float maxX = tileSize * (floorf((panel.x + panel.width) / tileSize) + 3);
    for (float lineX = minX; lineX < maxX; lineX += tileSize) {
        Color color;
        if (lineX == 0)  color = AXIS_COLOR;
        else color = LINE_COLOR;
        DrawLine(lineX, panel.y, lineX, panel.y + panel.height, color);
    }

    // Draw horizontal lines
    float minY = tileSize * (floorf(panel.y / tileSize) - 2);
    float maxY = tileSize * (floorf((panel.y + panel.height) / tileSize) + 3);
    for (float lineY = minY; lineY < maxY; lineY += tileSize) {
        Color color;
        if (lineY == 0) color = AXIS_COLOR;
        else color = LINE_COLOR;
        DrawLine(panel.x, lineY, panel.x + panel.width, lineY, color);
    }
}

void HandleLeftClick(void) {
    Vector2 mousePos = GetMousePosition();
    Vector2 mouseWorldPos = GetScreenToWorld2D(mousePos, editorCam);
    Vector2 snappedPos = SnapToGrid(mouseWorldPos, Vector2Zero(), SPACING);
    switch (clickMode) {
        case CLICK_SELECT:
            break;
        case CLICK_CREATE:
            bool mouseInEntity = false;
            for (int i = 0; i < numEntities; i++) {
                if (CheckCollisionPointRec(mouseWorldPos, entities[i]->rect)) {
                    mouseInEntity = true;
                    printf("Error: Cannot overlap entities\n");
                    break;
                }
            }
            if (!mouseInEntity) {
                switch (tool) {
                    case CREATE_OR:
                        NewGate(GateOR, snappedPos, SPACING, SPACING, 0.0f);
                        printf("OR created\n");
                        break;
                    case CREATE_AND:
                        NewGate(GateAND, snappedPos, SPACING, SPACING, 0.0f);
                        printf("AND created\n");
                        break;
                }
            }
            break;
        default:
            printf("Error: Invalid mode\n");
            exit(0);
    }
}

void UpdateEditor(void) {
    if (IsWindowResized()) {
        toolPanel.height = GetScreenHeight();
        editorPanel.width = GetScreenWidth() - toolPanel.width;
        editorPanel.height = GetScreenHeight();
    }
    Vector2 mousePos = GetMousePosition();
    Vector2 mouseWorldPos = GetScreenToWorld2D(mousePos, editorCam);
    Vector2 mouseDelta = GetMouseDelta();
    if (CheckCollisionPointRec(mousePos, editorPanel)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = Vector2Scale(mouseDelta, -1.0f / editorCam.zoom);
            editorCam.target = Vector2Add(editorCam.target, delta);
        }
        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            if (IsKeyPressed(KEY_ONE)) {
                clickMode = CLICK_SELECT;
                printf("New mode selected: Select\n");
            } else if (IsKeyPressed(KEY_TWO)) {
                clickMode = CLICK_CREATE;
                printf("New mode selected: Create\n");
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            printf("LMB pressed\n");
            HandleLeftClick();
        }
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            const float ZOOM_INCREMENT = 0.25f;
            const float MIN_ZOOM = 0.25f;
            const float MAX_ZOOM = 8.0f;

            editorCam.offset = mousePos;
            editorCam.target = mouseWorldPos;
            float targetZoom = editorCam.zoom + wheel * ZOOM_INCREMENT;
            editorCam.zoom = Clamp(targetZoom, MIN_ZOOM, MAX_ZOOM);
        }
    }
}

void DrawEditor(void) {
    BeginMode2D(editorCam);

        BeginScissorMode(   editorPanel.x, editorPanel.y, 
                            editorPanel.width, editorPanel.height);

            const Color EDITOR_BG_COLOR = BLACK;
            ClearBackground(EDITOR_BG_COLOR);

            // Get world space coordinates of the panel
            Vector2 topLeft = GetScreenToWorld2D(
                CLITERAL(Vector2) {editorPanel.x, editorPanel.y}, editorCam
            );
            Vector2 bottomRight = GetScreenToWorld2D(
                CLITERAL(Vector2) {
                    editorPanel.x + editorPanel.width,
                    editorPanel.y + editorPanel.height
                },
                editorCam
            );
            Rectangle panelWorld = {
                topLeft.x,
                topLeft.y,
                bottomRight.x - topLeft.x,
                bottomRight.y - topLeft.y
            };

            // Draw grid lines in world space
            DrawGridLines(panelWorld, SPACING);
            // Draw components
            for (int i = 0; i < numEntities; i++) {
                Entity e = *(entities[i]);
                Vector2 topLeftWorld = CLITERAL(Vector2) {
                    e.rect.x, e.rect.y
                };
                Vector2 bottomRightWorld = CLITERAL(Vector2) {
                    e.rect.x + e.rect.width,
                    e.rect.y + e.rect.height
                };
                Vector2 topLeft = GetWorldToScreen2D(
                    topLeftWorld, editorCam
                );
                Vector2 bottomRight = GetWorldToScreen2D(
                    bottomRightWorld, editorCam
                );
                Rectangle entityWorldRec = CLITERAL(Rectangle) {
                    topLeft.x, topLeft.y,
                    bottomRight.x - topLeft.x, bottomRight.y - topLeft.y
                };
                if (CheckCollisionRecs(entityWorldRec, editorPanel)) {
                    DrawEntity(entities[i], WHITE);
                    // DrawRectangleLinesEx(entityRec, 2, RED);
                }
            }

        EndScissorMode();

    EndMode2D();
    DrawRectangleLinesEx(editorPanel, borderThickness, borderColor);
    // test button --------------------
    if (CheckCollisionPointRec(GetMousePosition(), testButton)) {
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
    DrawRectangleLinesEx(testButton, 3.0, BLACK);
    // --------------------------------
    DrawToolbar(borderThickness, borderColor);
}