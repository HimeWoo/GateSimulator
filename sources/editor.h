#ifndef EDITOR_H
#define EDITOR_H

#include <raylib.h>
#include <raymath.h>
#include "entity.h"
#include "toolbar.h"
#include "button.h"
#include "gates.h"

    typedef enum ClickMode {
        CLICK_SELECT,
        CLICK_CREATE
    } ClickMode;

    typedef enum ToolMode {
        CREATE_OR,
        CREATE_AND
    } ToolMode;

    static Rectangle editorPanel;
    static Camera2D editorCam;
    static Rectangle testButton;
    static Color testButtonColor;
    static Texture2D testTexture;
    
    // Initialize editor panels and entity array
    void InitEditor(void);

    // Return the position snapped to the grid
    Vector2 SnapToGrid(Vector2 pos, Vector2 offset, int tileSize);

    // Draw grid lines within the bounds
    void DrawGridLines(Rectangle panel, int tileSize);

    // Logic for left mouse button event
    void HandleLeftClick(void);

    // Update entities and camera
    void UpdateEditor(void);

    // Draw the editor and entities
    void DrawEditor(void);

#endif