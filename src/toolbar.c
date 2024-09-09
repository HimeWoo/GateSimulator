#include "toolbar.h"

Rectangle toolPanel;

void InitToolbar(void)
{
  toolPanel = CLITERAL(Rectangle){0, 0, 50, GetScreenHeight()};
}

void DrawToolbar(float borderThickness, Color borderColor)
{
  DrawRectangleLinesEx(toolPanel, borderThickness, borderColor);
}
