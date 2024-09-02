#include "entity.h"

Entity **entities;
int numEntities;

void InitEntities(void)
{
  entities = malloc(0);
  numEntities = 0;
}

// Allocates memory for and returns a new entity
Entity *NewEntity(Vector2 pos, int width, int height, Texture2D tex, float rot)
{
  Entity *e = malloc(sizeof(Entity));
  e->rectangle = CLITERAL(Rectangle){ pos.x, pos.y, width, height };
  e->texture = tex;
  e->rotation = rot;
  numEntities++;
  entities = realloc(entities, numEntities * sizeof(Entity));
  entities[numEntities - 1] = e;
  printf("NewEntity, # of entities: %d\n", numEntities);
  return e;
}

// Draw an entity
void DrawEntity(Entity *e, Color tint)
{
  Rectangle texRectangle = CLITERAL(Rectangle)
    { 0, 0, e->texture.width, e->texture.height };
  DrawTexturePro(e->texture, texRectangle, e->rectangle,
                 Vector2Zero(), e->rotation, tint);
}

// Print entity information
void PrintEntity(Entity *e)
{
  printf("Entity: <%p> ---------------------------\n", &e);
  printf("| pos: (%.2f, %.2f)\n", e->rectangle.x, e->rectangle.y);
  printf("| dim: %.2f x %.2f\n", e->rectangle.width, e->rectangle.height);
  printf("| rot: %.2f\n", e->rotation);
}