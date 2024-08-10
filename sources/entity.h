#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
    
    typedef struct Entity {
        Vector2 pos;
        Texture2D tex;
        float rot;
        float scale;
    } Entity;

    // All loaded entities
    extern Entity **entities;
    extern int numEntities;

    Entity *NewEntity(Vector2 pos, Texture2D tex, float rot, float scale);
    void DrawEntity(Entity *e, Color tint);
    bool IsWithinEntity(Entity *e, Vector2 pos);
    void PrintEntityInfo(Entity *e);

#endif