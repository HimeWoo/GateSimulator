#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

typedef struct Button
{
  Rectangle rec;
  Texture2D tex;
  void (*OnPress)(void);
} Button;

Button *NewButton(Rectangle rec, Texture2D tex, void (*OnPress)(void));

#endif