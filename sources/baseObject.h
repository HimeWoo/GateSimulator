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
    void DrawObject(BaseObject* o, Color tint);
    void TranslateObject(BaseObject* o, Vector2 trans);
    bool IsWithinObject(BaseObject* o, Vector2 pos);
    void PrintObject(BaseObject* o);

#endif