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

static Entity *pressed = NULL;
static Entity *selected = NULL;

void InitEditor(void)
{
  InitToolbar();
  InitGateTypes();
  editorPanel = CLITERAL(Rectangle){
      toolPanel.width, 0,
      GetScreenWidth() - toolPanel.width, GetScreenHeight()};
  editorCam = CLITERAL(Camera2D){
      CLITERAL(Vector2){
          editorPanel.x + editorPanel.width / 2,
          editorPanel.y + editorPanel.height / 2},
      CLITERAL(Vector2){0, 0},
      0,
      1};

  testButton = CLITERAL(Rectangle){
      toolPanel.x + borderThickness, toolPanel.y + borderThickness,
      toolPanel.width - 2 * borderThickness,
      toolPanel.width - 2 * borderThickness};
  testButtonColor = WHITE;
  testTexture = LoadTexture(ASSETS_PATH "testimage.png");
}

Vector2 SnapToGrid(Vector2 pos, Vector2 offset, int tileSize)
{
  float outX = floorf(pos.x / tileSize + offset.x) * tileSize;
  float outY = floorf(pos.y / tileSize + offset.y) * tileSize;
  return CLITERAL(Vector2){ outX, outY };
}

Rectangle GetWorldToScreenRec(Rectangle recWorld, Camera2D cam)
{
  Vector2 topLeftWorld = CLITERAL(Vector2){ recWorld.x, recWorld.y };
  Vector2 botRightWorld = CLITERAL(Vector2){ 
      recWorld.x + recWorld.width, recWorld.y + recWorld.height};
  Vector2 topLeft = GetWorldToScreen2D(topLeftWorld, cam);
  Vector2 botRight = GetWorldToScreen2D(botRightWorld, cam);
  Rectangle out = CLITERAL(Rectangle){
      topLeft.x, topLeft.y, 
      botRight.x - topLeft.x, botRight.y - topLeft.y};
  return out;
}

Rectangle GetScreenToWorldRec(Rectangle recScreen, Camera2D cam)
{
  Vector2 topLeftScreen = CLITERAL(Vector2){ recScreen.x, recScreen.y };
  Vector2 topLeftWorld = GetScreenToWorld2D(topLeftScreen, editorCam);
  Vector2 botRightScreen = CLITERAL(Vector2){
    recScreen.x + recScreen.width, recScreen.y + recScreen.height};
  Vector2 botRightWorld = GetScreenToWorld2D(botRightScreen, editorCam);
  Rectangle out = CLITERAL(Rectangle){
    topLeftWorld.x, topLeftWorld.y,
    botRightWorld.x - topLeftWorld.x, botRightWorld.y - topLeftWorld.y};
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

void UpdateEditor(void)
{
  const Vector2 mousePos = GetMousePosition();
  const Vector2 mouseWorld = GetScreenToWorld2D(mousePos, editorCam);
  const Vector2 mouseDelta = GetMouseDelta();
  const Vector2 snappedPos = SnapToGrid(mouseWorld, Vector2Zero(), SPACING);

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
        clickMode = CLICK_SELECT;
        printf("New mode selected: Select\n");
      }
      else if (IsKeyPressed(KEY_TWO))
      {
        clickMode = CLICK_CREATE;
        printf("New mode selected: Create\n");
      }
    }
    // On LMB Press
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      printf("LMB pressed\n");
      switch (clickMode)
      {
        case CLICK_SELECT:
        {
          /* for (int i = 0; i < numEntities; i++)
          {
            if (CheckCollisionPointRec(mouseWorld, entities[i]->rectangle))
            {
              pressed = entities[i];
              printf("Pressed entity: <%p>\n", pressed);
              break;
            }
          } */
          if (selected == NULL)
          {
            for (int i = 0; i < numEntities; i++)
            {
              if (CheckCollisionPointRec(mouseWorld, entities[i]->rectangle))
              {
                selected = entities[i];
                printf("Selected entity: <%p>\n", selected);
                break;
              }
            }
          }
          break;
        }
        case CLICK_CREATE:
        {
          // Check if another entity is being clicked
          bool mouseInEntity = false;
          for (int i = 0; i < numEntities; i++)
          {
            if (CheckCollisionPointRec(mouseWorld, entities[i]->rectangle))
            {
              mouseInEntity = true;
              printf("Error: Cannot overlap entities\n");
              break;
            }
          }

          // If another entity is not being clicked, then create a gate
          if (!mouseInEntity)
          {
            switch (tool)
            {
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
        }
        default:
        {
          printf("Error: Invalid mode\n");
          exit(0);
        }
      }
    }

    // On LMB release
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
      printf("LMB released\n");
      switch (clickMode)
      {
        case CLICK_SELECT:
        {
          /* Entity *released = NULL;
          for (int i = 0; i < numEntities; i++)
          {
            if (CheckCollisionPointRec(mouseWorld, entities[i]->rectangle))
            {
              released = entities[i];
              break;
            }
          }
          if (released != NULL && released == pressed)
          {
            if (released == selected)
            {
              selected = NULL;
              printf("Deselected\n");
            }
            else
            {
              selected = released;
              printf("New selection : <%p>\n", selected);
            }
            break;
          }
          pressed = NULL; */
          break;
        }
        case CLICK_CREATE:
        {
          break;
        }
        default:
        {
          printf("Error: Invalid mode\n");
          exit(0);
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
      DrawGridLines(editorPanel, SPACING);
      // Draw components
      for (int i = 0; i < numEntities; i++)
      {
        Entity *e = entities[i];
        if (selected == e)
        {
          continue;
        }
        Rectangle recWorld = GetWorldToScreenRec(e->rectangle, editorCam);
        if (CheckCollisionRecs(recWorld, editorPanel))
        {
          DrawEntity(e, WHITE);
          /* if (e == selected)
          {
            DrawRectangleLinesEx(e->rectangle, 1.0f, RED);
          } */
        }
      }

      // Draw entity preview
      Vector2 mousePos = GetMousePosition();
      Vector2 mouseWorld = GetScreenToWorld2D(mousePos, editorCam);
      if (selected != NULL)
      {
        const Color alpha50 = (Color){ 255, 255, 255, 127 };
        Rectangle texRectangle = CLITERAL(Rectangle){
            0, 0, selected->texture.width, selected->texture.height };
        float recWidth = selected->rectangle.width;
        float recHeight = selected->rectangle.height;
        Vector2 offsetPos = CLITERAL(Vector2){
            mouseWorld.x - (recWidth / 2), mouseWorld.y - (recHeight / 2)};
        Rectangle mouseRectangle = CLITERAL(Rectangle){
            offsetPos.x, offsetPos.y,
            recWidth, recHeight};
        DrawTexturePro(selected->texture, texRectangle, mouseRectangle,
                       Vector2Zero(), selected->rotation, alpha50);
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