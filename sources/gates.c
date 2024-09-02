#include "gates.h"

// ----------------------------------------------------------------------------
// Gate definitions
// ----------------------------------------------------------------------------

GateType GateNULL;
GateType GateOR;
GateType GateAND;

void InitGateTypes(void)
{
  Rectangle outline = CLITERAL(Rectangle){0, 0, 128, 128};

  Image orImage = GenImageColor(128, 128, BLANK);
  ImageDrawRectangleLines(&orImage, outline, 8.0f, WHITE);
  ImageDrawText(&orImage, "OR", 16, 16, 48, WHITE);
  GateOR.tex = LoadTextureFromImage(orImage);
  GateOR.Op = &opOR;
  UnloadImage(orImage);

  Image andImage = GenImageColor(128, 128, BLANK);
  ImageDrawRectangleLines(&andImage, outline, 8.0f, WHITE);
  ImageDrawText(&andImage, "AND", 16, 16, 48, WHITE);
  GateAND.tex = LoadTextureFromImage(andImage);
  GateAND.Op = &opAND;
  UnloadImage(andImage);
}

Gate *NewGate(GateType t, Vector2 pos, int width, int height, float rot)
{
  Gate *g = malloc(sizeof(Gate));
  g->ent = NewEntity(pos, width, height, t.tex, rot);
  g->state = ERR;
  g->in = malloc(0);
  g->numIn = 0;
  g->out = malloc(0);
  g->numOut = 0;
  g->Op = t.Op;
  return g;
}

GateState opNULL(Gate *g)
{
  GateState out;
  for (int i = 0; i < g->numIn; i++)
  {
    out = g->in[0]->state;
    if (g->in[i]->state == ERR || g->in[i]->state != out)
    {
      return ERR;
    }
  }
  g->state = out;
  return out;
}

GateState opOR(Gate *g)
{
  GateState out = OFF;
  for (int i = 0; i < g->numIn; i++)
  {
    if (g->in[i]->state == ERR)
    {
      return ERR;
    }
    else if (g->in[i]->state == ON)
    {
      out = ON;
    }
  }
  g->state = out;
  return out;
}

GateState opAND(Gate *g)
{
  GateState out = ON;
  for (int i = 0; i < g->numIn; i++)
  {
    if (g->in[i]->state == ERR)
    {
      return ERR;
    }
    else if (g->in[i]->state == OFF)
    {
      out = OFF;
    }
  }
  g->state = out;
  return out;
}
