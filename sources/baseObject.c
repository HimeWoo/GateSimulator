#include "object.h"

// Allocates memory for and returns a new BaseObject
Object* NewObject(Vector2 pos, Texture2D tex, float rot, float scale) {
    Object* o = malloc(sizeof(Object));
    o->pos = pos;
    o->tex = tex;
    o->rot = rot;
    o->scale = scale;
    return o;
}

// Draw an object
void DrawObject(Object* o, Color tint) {
    DrawTextureEx(o->tex, o->pos, o->rot, o->scale, tint);
}

// Translate an object
void TranslateObject(Object* obj, Vector2 trans) {
    obj->pos = Vector2Add(obj->pos, trans);
}

// Check if position is within the object's texture
bool IsWithinObject(Object* o, Vector2 pos) {
    Rectangle texRec = CLITERAL(Rectangle){
        o->pos.x,
        o->pos.y,
        o->tex.width * o->scale,
        o->tex.height * o->scale
    };
    return CheckCollisionPointRec(pos, texRec);
}

// Print object information
void PrintObject(Object* o) {
    printf("Object: <%p> ---------------------------\n", &o);
    printf("| pos: (%.2f, %.2f)\n", o->pos.x, o->pos.y);
    printf("| rot: %.2f\n", o->rot);
    printf("| scale: %.2f\n", o->scale);
}