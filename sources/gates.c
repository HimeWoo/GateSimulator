#include "gates.h"

// ----------------------------------------------------------------------------
// Gate definitions
// ----------------------------------------------------------------------------

GateType GateOR;
GateType GateAND;

void InitGateTypes(void) {
    GateOR.tex = LoadTextureFromImage(GenImageColor(64, 64, BLUE));
    GateOR.op = &opOR;
    GateAND.tex = LoadTextureFromImage(GenImageColor(64, 64, PURPLE));
    GateAND.op = &opAND;
}

Gate *NewGate(GateType t, Vector2 pos, float rot, float scale) {
    Gate *g = malloc(sizeof(Gate));
    g->ent = NewEntity(pos, t.tex, rot, scale);
    g->state = ERR;
    g->in = malloc(0);
    g->numIn = 0;
    g->out = malloc(0);
    g->numOut = 0;
    g->op = t.op;
    return g;
}

GateState opNULL(Gate *g) {
    GateState out;
    for (int i = 0; i < g->numIn; i++) {
        out = g->in[0]->state;
        if (g->in[i]->state == ERR || g->in[i]->state != out) {
            return ERR;
        }
    }
    g->state = out;
    return out;
}

GateState opAND(Gate *g) {
    GateState out = ON;
    for (int i = 0; i < g->numIn; i++) {
        if (g->in[i]->state == ERR) {
            return ERR;
        } else if (g->in[i]->state == OFF) {
            out = OFF;
        }
    }
    g->state = out;
    return out;
}

GateState opOR(Gate *g) {
    GateState out = OFF;
    for (int i = 0; i < g->numIn; i++) {
        if (g->in[i]->state == ERR) {
            return ERR;
        } else if (g->in[i]->state == ON) {
            out = ON;
        }
    }
    g->state = out;
    return out;
}