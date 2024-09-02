#ifndef GATES_H
#define GATES_H

#include "entity.h"

typedef enum GateState
{
  ERR,
  OFF,
  ON
} GateState;

typedef struct Gate Gate;
struct Gate
{
  Entity *ent;
  GateState state;
  Gate **in;
  int numIn;
  Gate **out;
  int numOut;
  GateState (*Op)(Gate *);
};

typedef struct GateType
{
  Texture2D tex;
  GateState (*Op)(Gate *);
} GateType;

extern GateType GateOR;
extern GateType GateAND;

void InitGateTypes(void);
Gate *NewGate(GateType t, Vector2 pos, int width, int height, float rot);
GateState opNULL(Gate *g);
GateState opOR(Gate *g);
GateState opAND(Gate *g);

#endif