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
        Entity obj;
        GateState state;
        Gate **in;
        int numIn;
        Gate **out;
        int numOut;
        GateState (*op)(Gate);
    };

#endif