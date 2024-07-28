#ifndef EDITOR_H
#define EDITOR_H

#include <raylib.h>
#include <raymath.h>

    // Draw editor component
    void DrawComponent(Texture2D tex, Vector2 pos, float rot) {
        DrawTextureV(tex, pos, WHITE);
    }

    // Return the position snapped to the grid
    Vector2 SnapToGrid(Vector2 pos, float spacing, Vector2 offset) {
        Vector2 snapPos = {roundf(pos.x / spacing) * spacing, roundf(pos.y / spacing) * spacing};
        return Vector2Add(snapPos, offset);
    }

    // Draw grid lines within the bounds
    void DrawGridLines(Rectangle panel, float spacing) {
        // Color of the x=0 and y=0 lines
        const Color ZERO_LINE_COLOR = CLITERAL(Color) {0x00, 0x40, 0x00, 255};
        // Color of the other grid lines
        const Color LINE_COLOR = CLITERAL(Color) {0x00, 0x20, 0x00, 255};

        // Draw vertical lines
        float minX = spacing * (floorf(panel.x / spacing) - 2);
        float maxX = spacing * (floorf((panel.x + panel.width) / spacing) + 3);
        for (float lineX = minX; lineX < maxX; lineX += spacing) {
            Color color;
            if (lineX == 0)  color = ZERO_LINE_COLOR;
            else color = LINE_COLOR;
            DrawLine(lineX, panel.y, lineX, panel.y + panel.height, color);
        }

        // Draw horizontal lines
        float minY = spacing * (floorf(panel.y / spacing) - 2);
        float maxY = spacing * (floorf((panel.y + panel.height) / spacing) + 3);
        for (float lineY = minY; lineY < maxY; lineY += spacing) {
            Color color;
            if (lineY == 0) color = ZERO_LINE_COLOR;
            else color = LINE_COLOR;
            DrawLine(panel.x, lineY, panel.x + panel.width, lineY, color);
        }
    };

    // Draw the editor to texture
    void DrawEditor(Rectangle panel, Camera2D *cam) { 
        Vector2 mousePos = GetMousePosition();
        Vector2 mouseDelta = GetMouseDelta();
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = Vector2Scale(mouseDelta, -1.0f / cam->zoom);
            cam->target = Vector2Add(cam->target, delta);
        }
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            // Mouse position wrt the editor
            Vector2 mouseEditorPos = CLITERAL(Vector2) {
                mousePos.x - panel.x,
                mousePos.y - panel.y
            };
            Vector2 mouseWorldPos = GetScreenToWorld2D(mouseEditorPos, *cam);
            cam->offset = mouseEditorPos;
            cam->target = mouseWorldPos;
            const float ZOOM_INCREMENT = 0.05f;
            const float MIN_ZOOM = 0.1f;
            const float MAX_ZOOM = 2.0f;
            float targetZoom = cam->zoom + wheel * ZOOM_INCREMENT;
            cam->zoom = Clamp(targetZoom, MIN_ZOOM, MAX_ZOOM);
        }

        BeginMode2D(*cam);

            BeginScissorMode(panel.x, panel.y, panel.width, panel.height);

                const Color EDITOR_BG_COLOR = BLACK;
                ClearBackground(EDITOR_BG_COLOR);

                // Get world space coordinates of the panel
                Vector2 topLeft = GetScreenToWorld2D(
                    CLITERAL(Vector2) {panel.x, panel.y}, *cam
                );
                Vector2 bottomRight = GetScreenToWorld2D(
                    CLITERAL(Vector2) {
                        panel.x + panel.width,
                        panel.y + panel.height
                    },
                    *cam
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
                DrawGridLines(panelWorld, SPACING);
                // Draw components
                Vector2 mouseWorldPos = GetScreenToWorld2D(mousePos, *cam);
                DrawCircleV(SnapToGrid(mouseWorldPos, SPACING, Vector2Zero()), 10, WHITE);

            EndScissorMode();

        EndMode2D();

    }

#endif