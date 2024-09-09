#ifndef EDITOR_H
#define EDITOR_H

#include <raylib.h>
#include <raymath.h>
#include "entity.h"
#include "entityTypes.h"
#include "toolbar.h"

typedef enum ClickMode
{
  CLICK_SELECT,
  CLICK_CREATE
} ClickMode;

static Rectangle editorPanel;
static Camera2D editorCam;
static Rectangle testButton;
static Color testButtonColor;
static Texture2D testTexture;

// Initialize editor panels and entity array
void InitEditor(void);

// Return the position snapped to the grid
Vector2 SnapToGrid(Vector2 pos, int tileSize);

// Get the screen space rectangle for a 2d camera world space rectangle
Rectangle GetWorldToScreenRec(Rectangle recWorld, Camera2D cam);

// Get the 2d camera world space rectangle for a screen space rectangle
Rectangle GetScreenToWorldRec(Rectangle recScreen, Camera2D cam);

// Draw grid lines within the bounds
void DrawGridLines(Rectangle recScreen, int tileSize);

// Returns the entity at the given position, otherwise returns NULL
Entity *GetEntity(Vector2 posSnap);

// Update entities and camera
void UpdateEditor(void);

// Draw the editor and entities
void DrawEditor(void);

#endif