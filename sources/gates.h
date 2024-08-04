#ifndef GATES_H
#define GATES_H

#include "entity.h"

    typedef enum {
        OFF,
        ON,
        ERR
    } GateState;

    typedef struct Gate {
        Entity obj;
        GateState state;
        Entity **in;
        Entity **out;
    } Gate;

#endif