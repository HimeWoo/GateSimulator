#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>

    typedef struct BaseObject {
        Vector2 pos;
        Texture2D tex;
        float rot;
        float scale;
    } BaseObject;

    BaseObject* NewObject(Vector2 pos, Texture2D tex, float rot, float scale);
    void DrawObject(BaseObject* obj, Color tint);
    void TranslateObject(BaseObject* obj, Vector2 trans);
    bool IsWithinObject(BaseObject* obj, Vector2 pos);
    void PrintObject(BaseObject* obj);

#endif