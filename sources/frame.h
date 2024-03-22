#ifndef FRAME_H
#define FRAME_H

#include <raylib.h>
#include <raymath.h>

    typedef struct Frame {
        Vector2 pos;
        float w;
        float h;
        Camera2D *cam;
    } Frame;

    void DrawFrameBorder(Frame f, Color clr);
    bool IsWithinFrame(Frame f, Vector2 pos);

#endif