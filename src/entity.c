#include "entity.h"
#include <raymath.h>

int numEntities;
Entity **entities;

void InitEntities(void)
{
  entities = malloc(0);
  numEntities = 0;

  InitEntityTypes();
}

// Allocates memory for and returns a new entity
Entity *NewEntity(EntityType t, Vector2 pos, Vector2 texOffset,
                  float texWidth, float texHeight, float rot)
{
  Entity *e = malloc(sizeof(Entity));
  e->position = pos;
  e->texture = t.texture;
  e->textureOffset = texOffset;
  e->width = texWidth;
  e->height = texHeight;
  e->rotation = rot;
  e->numIn = 0;
  e->in = malloc(0);
  e->numOut = 0;
  e->out = malloc(0);

  numEntities++;
  entities = realloc(entities, numEntities * sizeof(Entity));
  entities[numEntities - 1] = e;
  //printf("NewEntity, # of entities: %d\n", numEntities);
  return e;
}

Rectangle GetTextureRec(Entity e)
{
  return CLITERAL(Rectangle){
      e.position.x - e.textureOffset.x,
      e.position.y - e.textureOffset.y,
      e.width,
      e.height};
}

// Draw an entity
void DrawEntity(Entity e, Color tint)
{
  Rectangle textureSrc = CLITERAL(Rectangle){
      0,
      0,
      e.texture.width,
      e.texture.height};
  Rectangle textureDest = CLITERAL(Rectangle){
      e.position.x,
      e.position.y,
      e.width,
      e.height};
  DrawTexturePro(e.texture, textureSrc, textureDest,
                 e.textureOffset, e.rotation, tint);
}

/* // Returns true if connection successful
bool ConnectEntities(Entity *transmit, Entity *receive)
{
  transmit->out = realloc(transmit->out, transmit->numOut + 1);
  transmit->numOut++;
} */

// Print entity information
void PrintEntityInfo(Entity *e)
{
  printf("Entity: <%p> ---------------------------\n", e);
  printf("| entity pos:\t(%.2f, %.2f)\n", e->position.x, e->position.y);
  printf("| texture pos:\t(%.2f, %.2f)\n",
         e->position.x + e->textureOffset.x,
         e->position.y + e->textureOffset.y);
  printf("| dimensions:\t%.2f x %.2f\n", e->width, e->height);
  printf("| rotation:\t%.2f\n", e->rotation);
  printf("| inputs:\t%d\n", e->numIn);
  for (int i = 0; i < e->numIn; i++)
  {
    printf("| | %d. <%p>\n", i, e->in[i]);
  }
  printf("| outputs:\t%d\n", e->numIn);
  for (int i = 0; i < e->numOut; i++)
  {
    printf("| | %d. <%p>\n", i, e->out[i]);
  }
}