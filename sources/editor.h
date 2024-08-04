#ifndef EDITOR_H
#define EDITOR_H

#include <raylib.h>
#include <raymath.h>
#include "entity.h"

    // Initialize editor panels and entity array
    void InitEditor(void);

    // Return the position snapped to the grid
    Vector2 SnapToGrid(Vector2 pos, Vector2 offset);

    // Draw grid lines within the bounds
    void DrawGridLines(Rectangle panel);

    // Update entities and camera
    void UpdateEditor();

    // Draw the editor and entities
    void DrawEditor();

#endif