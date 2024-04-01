#ifndef EDITOR_H
#define EDITOR_H

#include <raylib.h>
#include <raymath.h>

    // Draw grid lines within the bounds
    void DrawGridLines(Rectangle panel) {
        const Color editorBGColor = BLACK;
        ClearBackground(editorBGColor);

        // Color of the x=0 and y=0 lines
        const Color originLineColor = CLITERAL(Color) {0x00, 0x40, 0x00, 255};
        // Color of the other grid lines
        const Color lineColor = CLITERAL(Color) {0x00, 0x20, 0x00, 255};
        // Distance between grid lines at 1.0x zoom
        const float SPACE = 100;

        // Draw vertical lines
        float minX = floorf(panel.x / SPACE) * SPACE - 2 * SPACE;
        float maxX = floorf((panel.x + panel.width) / SPACE) * SPACE + 3 * SPACE;
        for (float lineX = minX; lineX < maxX; lineX += SPACE) {
            Color color;
            if (lineX == 0)  color = originLineColor;
            else color = lineColor;
            DrawLine(lineX, panel.y, lineX, panel.y + panel.height, color);
        }

        // Draw horizontal lines
        float minY = floorf(panel.y / SPACE) * SPACE - 2 * SPACE;
        float maxY = floorf((panel.y + panel.height) / SPACE) * SPACE + 3 * SPACE;
        for (float lineY = minY; lineY < maxY; lineY += SPACE) {
            Color color;
            if (lineY == 0) color = originLineColor;
            else color = lineColor;
            DrawLine(panel.x, lineY, panel.x + panel.width, lineY, color);
        }
    };
    
    // Draw the editor to texture
    void DrawEditor(RenderTexture2D tex, Rectangle panel, Camera2D cam) { 
        BeginTextureMode(tex);
            
            BeginMode2D(cam);

                // Get world space coordinates of the panel
                Vector2 topLeft = GetScreenToWorld2D(
                    CLITERAL(Vector2) {0, 0}, cam
                );
                Vector2 bottomRight = GetScreenToWorld2D(
                    CLITERAL(Vector2) {panel.width, panel.height}, cam
                );
                Rectangle panelWorld = {
                    topLeft.x,
                    topLeft.y,
                    bottomRight.x - topLeft.x,
                    bottomRight.y - topLeft.y
                };

                // Draw grid lines in world space
                DrawGridLines(panelWorld);
                // Draw components
                DrawCircle(100, 100, 10, WHITE);

            EndMode2D();

        EndTextureMode();

        // Draw the texture to screen
        DrawTextureRec(
            tex.texture,
            CLITERAL(Rectangle) {0, 0, panel.width, -panel.height},
            CLITERAL(Vector2) {panel.x, panel.y},
            WHITE
        );
    }



#endif