#include "editor.h"

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

const int TILE_SIZE = 16;
const float borderThickness = 2.0f;
const Color borderColor = WHITE;

// ----------------------------------------------------------------------------
// Editor variables
// ----------------------------------------------------------------------------

static EditorMode editorMode = MODE_SELECT;
static EntityType createTarget;
static Entity *selected = NULL;

void InitEditor(void)
{
  InitToolbar();
  InitEntityTypes(TILE_SIZE);
  InitEntities();
  editorPanel = CLITERAL(Rectangle){
      toolPanel.width,
      0,
      GetScreenWidth() - toolPanel.width,
      GetScreenHeight()};
  editorCam = CLITERAL(Camera2D){
      CLITERAL(Vector2){
          editorPanel.x + editorPanel.width / 2,
          editorPanel.y + editorPanel.height / 2},
      Vector2Zero(),
      0,
      1};

  testButton = CLITERAL(Rectangle){
      toolPanel.x + borderThickness,
      toolPanel.y + borderThickness,
      toolPanel.width - 2 * borderThickness,
      toolPanel.width - 2 * borderThickness};
  testButtonColor = WHITE;
  testTexture = LoadTexture(ASSETS_PATH "testimage.png");

  createTarget = GateNOT;

  Vector2 offset = CLITERAL(Vector2){ TILE_SIZE / 2, TILE_SIZE / 2 };
  Entity *switch1 = NewEntity(IOSwitch, CLITERAL(Vector2){ 0, 0 },
            offset, TILE_SIZE, TILE_SIZE, 0.0f);
  Entity *switch2 = NewEntity(IOSwitch, CLITERAL(Vector2){ 0, TILE_SIZE * 2 },
            offset, TILE_SIZE, TILE_SIZE, 0.0f);
  Entity *and = NewEntity(GateAND, CLITERAL(Vector2){ TILE_SIZE * 2, 0 },
            offset, TILE_SIZE * 3, TILE_SIZE * 3, 0.0f);
  ConnectEntities(switch1, and);
  ConnectEntities(switch2, and);
}

Vector2 SnapToGrid(Vector2 pos, int tileSize)
{
  int outX = floorf((pos.x + tileSize / 2) / tileSize) * tileSize;
  int outY = floorf((pos.y + tileSize / 2) / tileSize) * tileSize;
  return CLITERAL(Vector2){ outX, outY };
}

Rectangle GetWorldToScreenRec(Rectangle recWorld, Camera2D cam)
{
  Vector2 topLeftWorld = CLITERAL(Vector2){
      recWorld.x,
      recWorld.y};
  Vector2 botRightWorld = CLITERAL(Vector2){
      recWorld.x + recWorld.width,
      recWorld.y + recWorld.height};
  Vector2 topLeft = GetWorldToScreen2D(topLeftWorld, cam);
  Vector2 botRight = GetWorldToScreen2D(botRightWorld, cam);
  Rectangle out = CLITERAL(Rectangle){
      topLeft.x,
      topLeft.y, 
      botRight.x - topLeft.x,
      botRight.y - topLeft.y};
  return out;
}

Rectangle GetScreenToWorldRec(Rectangle recScreen, Camera2D cam)
{
  Vector2 topLeftScreen = CLITERAL(Vector2){
      recScreen.x,
      recScreen.y};
  Vector2 topLeftWorld = GetScreenToWorld2D(topLeftScreen, editorCam);
  Vector2 botRightScreen = CLITERAL(Vector2){
      recScreen.x + recScreen.width,
      recScreen.y + recScreen.height};
  Vector2 botRightWorld = GetScreenToWorld2D(botRightScreen, editorCam);
  Rectangle out = CLITERAL(Rectangle){
      topLeftWorld.x,
      topLeftWorld.y,
      botRightWorld.x - topLeftWorld.x,
      botRightWorld.y - topLeftWorld.y};
  return out;
}

void DrawGridLines(Rectangle panel, int tileSize)
{
  Rectangle bound = GetScreenToWorldRec(panel, editorCam);
  const float minX = bound.x;
  const float maxX = bound.x + bound.width;
  const float minY = bound.y;
  const float maxY = bound.y + bound.height;

  // Color of the x=0 and y=0 lines
  const Color AXIS_COLOR = CLITERAL(Color){ 0x00, 0x40, 0x00, 255 };
  // Color of the other grid lines
  const Color LINE_COLOR = CLITERAL(Color){ 0x00, 0x20, 0x00, 255 };

  // Draw vertical lines
  float left = tileSize * (floorf(minX / tileSize) - 2);
  float right = tileSize * (floorf(maxX / tileSize) + 3);
  for (float x = left; x < right; x += tileSize)
  {
    Color color;
    if (x == 0)
    {
      color = AXIS_COLOR;
    }
    else
    {
      color = LINE_COLOR;
    }
    DrawLine(x, minY, x, maxY, color);
  }

  // Draw horizontal lines
  float bot = tileSize * (floorf(minY / tileSize) - 2);
  float top = tileSize * (floorf(maxY / tileSize) + 3);
  for (float y = bot; y < top; y += tileSize)
  {
    Color color;
    if (y == 0)
    {
      color = AXIS_COLOR;
    }
    else
    {
      color = LINE_COLOR;
    }
    DrawLine(minX, y, maxX, y, color);
  }
}

Entity *GetEntity(Vector2 posSnap)
{
  for (int i = 0; i < numEntities; i++)
  {
    if (Vector2Equals(entities[i]->position, posSnap))
    {
      return entities[i];
    }
  }
  return NULL;
}

void UpdateEditor(void)
{
  const Vector2 mousePos = GetMousePosition();
  const Vector2 mouseWorld = GetScreenToWorld2D(mousePos, editorCam);
  const Vector2 mouseDelta = GetMouseDelta();
  const Vector2 mouseSnap = SnapToGrid(mouseWorld, TILE_SIZE);

  if (IsWindowResized())
  {
    toolPanel.height = GetScreenHeight();
    editorPanel.width = GetScreenWidth() - toolPanel.width;
    editorPanel.height = GetScreenHeight();
  }
  if (CheckCollisionPointRec(mousePos, editorPanel))
  {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
      Vector2 delta = Vector2Scale(mouseDelta, -1.0f / editorCam.zoom);
      editorCam.target = Vector2Add(editorCam.target, delta);
    }
    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
      if (IsKeyPressed(KEY_ONE))
      {
        editorMode = MODE_SELECT;
        printf("New mode selected: Select\n");
      }
      else if (IsKeyPressed(KEY_TWO))
      {
        editorMode = MODE_CREATE;
        printf("New mode selected: Create\n");
      }
      else if (IsKeyPressed(KEY_R))
      {
        editorMode = MODE_SIMULATE;
        printf("New mode selected: Simulate\n");
      }
    }
    // On LMB Press
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      printf("LMB pressed\n");
      switch (editorMode)
      {
        case MODE_SELECT:
        {
          if (selected == NULL)
          {
            for (int i = 0; i < numEntities; i++)
            {
              Rectangle rec = GetTextureRec(*(entities[i]));
              if (CheckCollisionPointRec(mouseWorld, rec))
              {
                selected = entities[i];
                printf("Selected entity: <%p>\n", selected);
                PrintEntityInfo(selected);
                break;
              }
            }
          }
          else
          {
            Entity *clicked = GetEntity(mouseSnap);
            if (clicked == NULL)
            {
              selected->position.x = mouseSnap.x;
              selected->position.y = mouseSnap.y;
            }
            selected = NULL;
          }
          break;
        }
        case MODE_CREATE:
        {
          // Check if another entity is being clicked
          Entity *collision = GetEntity(mouseSnap);
          if (collision == NULL)
          {
            Vector2 offset = CLITERAL(Vector2){
              TILE_SIZE / 2,
              TILE_SIZE / 2};
            Entity *e = NewEntity(createTarget, mouseSnap, offset,
                      3 * TILE_SIZE, 3 * TILE_SIZE, 0.0f);
            printf("Entity created\n");
            PrintEntityInfo(e);
          }
          else
          {
            printf("Entity already exists there\n");
          }
          break;
        }
        default:
        {
          printf("Error: Invalid mode\n");
          exit(0);
        }
        case MODE_SIMULATE:
        {
          Entity *collision = GetEntity(mouseSnap);
          if (collision != NULL && collision->type.onClick != NULL)
          {
            collision->type.onClick(collision);
          }
        }
      }
    }

    // On mouse wheel scroll
    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
      const float ZOOM_INCREMENT = 0.25f;
      const float MIN_ZOOM = 0.25f;
      const float MAX_ZOOM = 8.0f;

      editorCam.offset = mousePos;
      editorCam.target = mouseWorld;
      float targetZoom = editorCam.zoom + wheel * ZOOM_INCREMENT;
      editorCam.zoom = Clamp(targetZoom, MIN_ZOOM, MAX_ZOOM);
    }
  }
}

void DrawEditor(void)
{
  BeginMode2D(editorCam);

    BeginScissorMode(editorPanel.x, editorPanel.y,
                     editorPanel.width, editorPanel.height);

      const Color EDITOR_BG_COLOR = BLACK;
      ClearBackground(EDITOR_BG_COLOR);

      // Draw grid lines in world space
      DrawGridLines(editorPanel, TILE_SIZE);
      // Draw components
      for (int i = 0; i < numEntities; i++)
      {
        Entity *e = entities[i];
        if (selected == e)
        {
          continue;
        }
        Rectangle dest = GetTextureRec(*e);
        Rectangle recWorld = GetWorldToScreenRec(dest, editorCam);
        if (CheckCollisionRecs(recWorld, editorPanel))
        {
          DrawEntity(*e, WHITE);
          DrawRectangleLinesEx(dest, 2, RED);
          DrawCircleV(e->position, 2, BLUE);
          for (int i = 0; i < e->numOut; i++)
          {
            DrawLineEx(e->position, e->out[i]->position, 2.0f, GREEN);
          }
        }
      }

      // Draw entity preview
      Vector2 mousePos = GetMousePosition();
      Vector2 mouseWorld = GetScreenToWorld2D(mousePos, editorCam);
      Vector2 mouseSnap = SnapToGrid(mouseWorld, TILE_SIZE);
      DrawCircleV(mouseSnap, TILE_SIZE / 8, WHITE);
      if (selected != NULL)
      {
        const Color alpha50 = (Color){ 255, 255, 255, 127 };
        Rectangle textureSrc = CLITERAL(Rectangle){
            0,
            0,
            selected->type.textures[0].width,
            selected->type.textures[0].height};
        Rectangle mouseRectangle = GetTextureRec(*selected);
        mouseRectangle.x = mouseSnap.x;
        mouseRectangle.y = mouseSnap.y;
        DrawTexturePro(selected->type.textures[0], textureSrc,
                       mouseRectangle, selected->textureOffset,
                       selected->rotation, alpha50);
      }

    EndScissorMode();

  EndMode2D();
  DrawRectangleLinesEx(editorPanel, borderThickness, borderColor);
  // test button --------------------
  if (CheckCollisionPointRec(GetMousePosition(), testButton))
  {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
      testButtonColor = DARKGRAY;
    }
    else
    {
      testButtonColor = GRAY;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
      printf("Button clicked\n");
    }
  }
  DrawRectangleRec(testButton, testButtonColor);
  DrawRectangleLinesEx(testButton, 3.0f, BLACK);
  // --------------------------------
  DrawToolbar(borderThickness, borderColor);
}