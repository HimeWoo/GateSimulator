#ifndef ENTITYTYPES_H
#define ENTITYTYPES_H

#include <raylib.h>

typedef struct EntityType
{
  Texture2D texture;
  int defaultNumIn;
  int defaultNumOut;
} EntityType;

extern EntityType GateNOT;
extern EntityType GateOR;
extern EntityType GateAND;

void InitEntityTypes(void);

#endif