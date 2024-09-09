#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "entityTypes.h"

typedef struct Entity Entity;
struct Entity
{
  Vector2 position;
  float width;
  float height;
  Texture2D texture;
  Vector2 textureOffset;
  float rotation;
  int numIn;
  Entity **in;
  int numOut;
  Entity **out;
};

// All loaded entities
extern int numEntities;
extern Entity **entities;

void InitEntities(void);
Entity *NewEntity(EntityType t, Vector2 pos, Vector2 texOffset,
                  float texWidth, float texHeight, float rot);
Rectangle GetTextureRec(Entity e);
void DrawEntity(Entity e, Color tint);
void PrintEntityInfo(Entity *e);

#endif