#ifndef EDITOR_H
#define EDITOR_H

#include <raylib.h>
#include <raymath.h>
#include "entity.h"
#include "gates.h"

    static Rectangle toolPanel;
    static Rectangle editorPanel;
    static Camera2D editorCam;
    static Rectangle testButton;
    static Color testButtonColor;
    static Texture2D testTexture;
    
    // Initialize editor panels and entity array
    void InitEditor(void);

    // Return the position snapped to the grid
    Vector2 SnapToGrid(Vector2 pos, Vector2 offset);

    // Draw grid lines within the bounds
    void DrawGridLines(Rectangle panel);

    // Update entities and camera
    void UpdateEditor(void);

    // Draw the editor and entities
    void DrawEditor(void);

#endif