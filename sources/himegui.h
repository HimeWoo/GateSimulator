#ifndef HIMEGUI_H
#define HIMEGUI_H

#include <raylib.h>
#include <raymath.h>

    void DrawGrid2D(Rectangle panel, Camera2D cam, RenderTexture2D tex) {
        BeginTextureMode(tex);

            const Color editorBGColor = BLACK;
            ClearBackground(editorBGColor);

            BeginMode2D(cam);

                // Color of the grid lines
                const Color lineColor = CLITERAL(Color) {0x00, 0x20, 0x00, 255};
                // Color of the x=0 and y=0 lines
                const Color originLineColor = CLITERAL(Color) {0x00, 0x30, 0x00, 255};
                // Distance between grid lines
                const float SPACING = 100;
                // Bleed over the edge of the editor panel
                const float BLEED = 10;

                // World space coordinates of the corners of the panel
                Vector2 topLeft = GetScreenToWorld2D(
                    CLITERAL(Vector2) {0, 0}, cam
                );
                Vector2 bottomRight = GetScreenToWorld2D(
                    CLITERAL(Vector2) {panel.width, panel.height}, cam
                );

                // Draw vertical lines
                float left = floorf(topLeft.x / SPACING) * SPACING - 2 * SPACING;
                float right = floorf(bottomRight.x / SPACING) * SPACING + 3 * SPACING;
                for (float lineX = left; lineX < right; lineX += SPACING) {
                    Color col;
                    if (lineX == 0) {
                        col = originLineColor;
                    } else {
                        col = lineColor;
                    }
                    DrawLine(
                        lineX, topLeft.y - BLEED,
                        lineX, bottomRight.y + BLEED,
                        col
                    );
                }

                // Draw horizontal lines
                float top = floorf(topLeft.y / SPACING) * SPACING - 2 * SPACING;
                float bottom = floorf(bottomRight.y / SPACING) * SPACING + 3 * SPACING;
                for (float lineY = top; lineY < bottom; lineY += SPACING) {
                    Color col;
                    if (lineY == 0) {
                        col = originLineColor;
                    } else {
                        col = lineColor;
                    }
                    DrawLine(
                        topLeft.x - BLEED, lineY,
                        bottomRight.x + BLEED, lineY,
                        col
                    );
                }

            EndMode2D();

        EndTextureMode();
        DrawTextureRec(
            tex.texture,
            CLITERAL(Rectangle) {
                0, 0,
                panel.width, -panel.height
            },
            CLITERAL(Vector2) {
                panel.x,
                0
            },
            WHITE
        );
    };

    void DrawToolMenu(Rectangle panel) {

    };

#endif