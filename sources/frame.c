#include "frame.h"

// Draw lines on the edges of the frame
void DrawFrameBorder(Frame f, Color clr) {
    DrawLineV(  f.pos,
                Vector2Add(f.pos, (Vector2){0, f.h}), clr);
    DrawLineV(  Vector2Add(f.pos, (Vector2){f.w, 0}),
                Vector2Add(f.pos, (Vector2){f.w, f.h}), clr);
    DrawLineV(  f.pos,
                Vector2Add(f.pos, (Vector2){f.w, 0}), clr);
    DrawLineV(  Vector2Add(f.pos, (Vector2){0, f.h}),
                Vector2Add(f.pos, (Vector2){f.w, f.h}), clr);
}

// Returns true if pos is inside frame
bool IsWithinFrame(Frame f, Vector2 pos) {
    return  pos.x > f.pos.x && pos.x < f.pos.x + f.w &&
            pos.y > f.pos.y && pos.y < f.pos.y + f.h;
}
