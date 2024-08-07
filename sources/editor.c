#include "editor.h"
#include "entity.h"

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

const float toolBorderThickness = 2.0f;
const Color toolBorderColor = WHITE;
const int SPACING = 100;

// ------------------------------------------------------------------------
// Globals
// ------------------------------------------------------------------------
Entity **entities;
int numEntities;
Rectangle toolPanel;
Rectangle editorPanel;
Camera2D editorCam;
Rectangle testButton;
Color testButtonColor;
Texture2D testTexture;

void InitEditor(void) {
    entities = malloc(0);
    numEntities = 0;
    
    toolPanel = CLITERAL(Rectangle) {
        0, 0, 
        50, GetScreenHeight()
    };
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
        toolPanel.x + toolBorderThickness, toolPanel.y + toolBorderThickness,
        toolPanel.width - 2 * toolBorderThickness, toolPanel.width
    };
    testButtonColor = WHITE;
    testTexture = LoadTexture(ASSETS_PATH "testimage.png");
}

Vector2 SnapToGrid(Vector2 pos, Vector2 offset) {
    Vector2 snapPos = {
        roundf(pos.x / SPACING) * SPACING,
        roundf(pos.y / SPACING) * SPACING
    };
    return Vector2Add(snapPos, offset);
}

void DrawGridLines(Rectangle panel) {
    // Color of the x=0 and y=0 lines
    const Color AXIS_COLOR = CLITERAL(Color) {0x00, 0x40, 0x00, 255};
    // Color of the other grid lines
    const Color LINE_COLOR = CLITERAL(Color) {0x00, 0x20, 0x00, 255};

    // Draw vertical lines
    float minX = SPACING * (floorf(panel.x / SPACING) - 2);
    float maxX = SPACING * (floorf((panel.x + panel.width) / SPACING) + 3);
    for (float lineX = minX; lineX < maxX; lineX += SPACING) {
        Color color;
        if (lineX == 0)  color = AXIS_COLOR;
        else color = LINE_COLOR;
        DrawLine(lineX, panel.y, lineX, panel.y + panel.height, color);
    }

    // Draw horizontal lines
    float minY = SPACING * (floorf(panel.y / SPACING) - 2);
    float maxY = SPACING * (floorf((panel.y + panel.height) / SPACING) + 3);
    for (float lineY = minY; lineY < maxY; lineY += SPACING) {
        Color color;
        if (lineY == 0) color = AXIS_COLOR;
        else color = LINE_COLOR;
        DrawLine(panel.x, lineY, panel.x + panel.width, lineY, color);
    }
}

void UpdateEditor() {
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
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            printf("Mouse up: -------------------------------\n");
            printf("\t- Before:\tnumEntities = %d\n", numEntities);
            Vector2 snappedPos =  SnapToGrid(mouseWorldPos, Vector2Zero());
            Entity *e = NewEntity(snappedPos, testTexture, 0.0f, 0.1f);
            numEntities++;
            entities = realloc(entities, numEntities * sizeof(Entity));
            entities[numEntities - 1] = e;
            printf("\t- After:\tnumEntities = %d\n", numEntities);
        } 
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            const float ZOOM_INCREMENT = 0.05f;
            const float MIN_ZOOM = 0.1f;
            const float MAX_ZOOM = 2.0f;

            editorCam.offset = mousePos;
            editorCam.target = mouseWorldPos;
            float targetZoom = editorCam.zoom + wheel * ZOOM_INCREMENT;
            editorCam.zoom = Clamp(targetZoom, MIN_ZOOM, MAX_ZOOM);
        }
    }
}

void DrawEditor() {
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

            // Spacing between grid lines at 1.0x zoom
            const int SPACING = 100;
            // Draw grid lines in world space
            DrawGridLines(panelWorld);
            // Draw components
            for (int i = 0; i < numEntities; i++) {
                Entity e = *(entities[i]);
                Rectangle entityRec = CLITERAL(Rectangle) {
                    e.pos.x, e.pos.y,
                    e.scale * e.tex.width, e.scale * e.tex.height
                };
                Vector2 topLeftWorld = CLITERAL(Vector2) {
                    entityRec.x, entityRec.y
                };
                Vector2 bottomRightWorld = CLITERAL(Vector2) {
                    entityRec.x + entityRec.width,
                    entityRec.y + entityRec.height
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
                    printf("Entity <%d>: ---------\n", i);
                    printf(
                        "\t- top left:\t(%.2f, %.2f)\n",
                        entityWorldRec.x, 
                        entityWorldRec.y
                    );
                    DrawEntity(entities[i], WHITE);
                    DrawRectangleLinesEx(entityRec, 4, RED);
                }
            }

        EndScissorMode();

    EndMode2D();
    DrawRectangleLinesEx(editorPanel, toolBorderThickness, toolBorderColor);
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
    DrawRectangleLinesEx(toolPanel, toolBorderThickness, toolBorderColor);
}