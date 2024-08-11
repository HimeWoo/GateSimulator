#ifndef GATES_H
#define GATES_H

#include "entity.h"

    typedef enum {
        OFF,
        ON,
        ERR
    } GateState;

    typedef struct Gate Gate;
    struct Gate {
        Entity *ent;
        GateState state;
        Gate **in;
        int numIn;
        Gate **out;
        int numOut;
        GateState (*op)(Gate *);
    };

    typedef struct GateType {
        Texture2D tex;
        GateState (*op)(Gate *);
    } GateType;

    extern GateType GateOR;
    extern GateType GateAND;

    void InitGateTypes(void);
    Gate *NewGate(GateType t, Vector2 pos, float rot, float scale);
    GateState opOR(Gate *g);
    GateState opAND(Gate *g);

#endif