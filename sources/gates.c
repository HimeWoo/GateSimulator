#include "gates.h"
GateState opNULL(Gate g) {
    GateState out;
    for (int i = 0; i < g.numIn; i++) {
        out = g.in[0];
        if (g.in[i]->state == ERR || g.in[i]->state != out) {
            return ERR;
        }
    }
    g.state = out;
    return out;
}
GateState opAND(Gate g) {
    GateState out = ON;
    for (int i = 0; i < g.numIn; i++) {
        if (g.in[i]->state == ERR) {
            return ERR;
        } else if (g.in[i]->state == OFF) {
            out = OFF;
        }
    }
    g.state = out;
    return out;
}