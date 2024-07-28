#ifndef GATES_H
#define GATES_H

#include "object.h"

    typedef struct GateAND {
        Object o;
        Object **in;
    };

#endif