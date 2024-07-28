#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>

    typedef struct Object {
        Vector2 pos;
        Texture2D tex;
        float rot;
        float scale;
    } Object;

    Object* NewObject(Vector2 pos, Texture2D tex, float rot, float scale);
    void DrawObject(Object* o, Color tint);
    void TranslateObject(Object* o, Vector2 trans);
    bool IsWithinObject(Object* o, Vector2 pos);
    void PrintObject(Object* o);

#endif