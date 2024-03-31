#include "baseObject.h"

// Allocates memory for and returns a new BaseObject
BaseObject* NewObject(Vector2 pos, Texture2D tex, float rot, float scale) {
    BaseObject* o = malloc(sizeof(BaseObject));
    o->pos = pos;
    o->tex = tex;
    o->rot = rot;
    o->scale = scale;
    return o;
}

// Draw an object
void DrawObject(BaseObject* o, Color tint) {
    DrawTextureEx(o->tex, o->pos, o->rot, o->scale, tint);
}

// Translate an object
void TranslateObject(BaseObject* obj, Vector2 trans) {
    obj->pos = Vector2Add(obj->pos, trans);
}

// Check if position is within the object's texture
bool IsWithinObject(BaseObject* o, Vector2 pos) {
    Rectangle texRec = CLITERAL(Rectangle){
        o->pos.x,
        o->pos.y,
        o->tex.width * o->scale,
        o->tex.height * o->scale
    };
    return CheckCollisionPointRec(pos, texRec);
}

// Print object information
void PrintObject(BaseObject* o) {
    printf("Object: <%p> ---------------------------\n", &o);
    printf("| pos: (%.2f, %.2f)\n", o->pos.x, o->pos.y);
    printf("| rot: %.2f\n", o->rot);
    printf("| scale: %.2f\n", o->scale);
}