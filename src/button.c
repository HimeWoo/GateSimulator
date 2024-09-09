#include "button.h"

Button *NewButton(Rectangle rec, Texture2D tex, void (*OnPress)(void))
{
  Button *b = malloc(sizeof(Button));
  b->rec = rec;
  b->tex = tex;
  b->OnPress = OnPress;
  return b;
}