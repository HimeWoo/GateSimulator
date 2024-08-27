#include "entity.h"

Entity **entities;
int numEntities;

void InitEntities(void) {
    entities = malloc(0);
    numEntities = 0;
}

// Allocates memory for and returns a new entity
Entity *NewEntity(  Vector2 pos, int width, int height,
                    Texture2D tex, float rot) {
    Entity *e = malloc(sizeof(Entity));
    e->rect = CLITERAL(Rectangle) { pos.x, pos.y, width, height };
    e->tex = tex;
    e->rot = rot;
    numEntities++;
    entities = realloc(entities, numEntities * sizeof(Entity));
    entities[numEntities - 1] = e;
    printf("NewEntity, # of entities: %d\n", numEntities);
    return e;
}

// Draw an entity
void DrawEntity(Entity *e, Color tint) {
    Rectangle texRectangle = CLITERAL(Rectangle) {
        0, 0, e->tex.width, e->tex.height
    };
    DrawTexturePro(e->tex, texRectangle, e->rect, Vector2Zero(), e->rot, tint);
}

// Print entity information
void PrintEntity(Entity *e) {
    printf("Entity: <%p> ---------------------------\n", &e);
    printf("| pos: (%.2f, %.2f)\n", e->rect.x, e->rect.y);
    printf("| dim: %d x %d\n", e->rect.width, e->rect.height);
    printf("| rot: %.2f\n", e->rot);
    printf("| scale: %.2f\n", e->scale);
}