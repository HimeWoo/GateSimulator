#include "baseObject.h"

// Allocates memory for and returns a new BaseObject
BaseObject* NewObject(Vector2 pos, Texture2D tex, float rot, float scale) {
    BaseObject* obj = malloc(sizeof(BaseObject));
    obj->pos = pos;
    obj->tex = tex;
    obj->rot = rot;
    obj->scale = scale;
    return obj;
}

// Draw an object
void DrawObject(BaseObject* obj, Color tint) {
    DrawTextureEx(obj->tex, obj->pos, obj->rot, obj->scale, tint);
}

// Translate an object
void TranslateObject(BaseObject* obj, Vector2 trans) {
    obj->pos = Vector2Add(obj->pos, trans);
}

// Returns true if the position is inside the texture of the object
bool IsWithinObject(BaseObject* obj, Vector2 pos) {
    return  pos.x > obj->pos.x && 
            pos.x < obj->pos.x + obj->tex.width * obj->scale &&
            pos.y > obj->pos.y &&
            pos.y < obj->pos.y + obj->tex.height * obj->scale;
}

// Print object information
void PrintObject(BaseObject* obj) {
    printf("Object: <%p> ---------------------------\n", &obj);
    printf("| pos: (%.2f, %.2f)\n", obj->pos.x, obj->pos.y);
    printf("| rot: %.2f\n", obj->rot);
    printf("| scale: %.2f\n", obj->scale);
}