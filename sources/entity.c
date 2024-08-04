#include "entity.h"

// Allocates memory for and returns a new entity
Entity *NewEntity(Vector2 pos, Texture2D tex, float rot, float scale) {
    Entity *e = malloc(sizeof(Entity));
    e->pos = pos;
    e->tex = tex;
    e->rot = rot;
    e->scale = scale;
    return e;
}

// Draw an entity
void DrawEntity(Entity *e, Color tint) {
    DrawTextureEx(e->tex, e->pos, e->rot, e->scale, tint);
}

// Translate an entity
void TranslateEntity(Entity *e, Vector2 trans) {
    e->pos = Vector2Add(e->pos, trans);
}

// Check if position is within the entity's texture
bool IsWithinEntity(Entity *e, Vector2 pos) {
    Rectangle texRec = CLITERAL(Rectangle){
        e->pos.x,
        e->pos.y,
        e->tex.width * e->scale,
        e->tex.height * e->scale
    };
    return CheckCollisionPointRec(pos, texRec);
}

// Print entity information
void PrintEntity(Entity *e) {
    printf("Entity: <%p> ---------------------------\n", &e);
    printf("| pos: (%.2f, %.2f)\n", e->pos.x, e->pos.y);
    printf("| rot: %.2f\n", e->rot);
    printf("| scale: %.2f\n", e->scale);
}