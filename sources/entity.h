#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>

typedef struct Entity
{
  Rectangle rectangle;
  Texture2D texture;
  Vector2 textureOffset;
  float rotation;
} Entity;

// All loaded entities
extern int numEntities;
extern Entity **entities;

void InitEntities(void);
Entity *NewEntity(Vector2 pos, int width, int height, Texture2D tex,
                  float rot);
void DrawEntity(Entity *e, Color tint);
void PrintEntityInfo(Entity *e);

#endif