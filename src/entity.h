#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

typedef struct Entity Entity;
typedef struct EntityType EntityType;

// Entity type information
struct EntityType
{
  int numTextures;
  Texture2D *textures;
  void (*update)(Entity *);
  void (*onClick)(Entity *);
};

// Objects that exist within the editor
struct Entity
{
  Vector2 position;
  float width;
  float height;
  EntityType type;
  Vector2 textureOffset;
  float rotation;
  int numIn;
  Entity **in;
  int numOut;
  Entity **out;
};

// All loaded entities
extern int numEntities;
extern Entity **entities;

extern EntityType GateWire;
extern EntityType GateNOT;
extern EntityType GateOR;
extern EntityType GateAND;
extern EntityType IOSwitch;
extern EntityType IODisplay;

// Initialize default entity types
void InitEntityTypes(int tileSize);

// Initialize entity list and types
void InitEntities(void);

// Allocates memory for and returns a new entity
Entity *NewEntity(EntityType t, Vector2 pos, Vector2 texOffset,
                  float texWidth, float texHeight, float rot);

// Returns the world space rectangle of the entity's texture
Rectangle GetTextureRec(Entity e);

// Draw the given entity
void DrawEntity(Entity e, Color tint);

// Establishes connection between two given entities
void ConnectEntities(Entity *output, Entity *input);

// Print detailed information about the given entity
void PrintEntityInfo(Entity *e);

void IOSwitch_onClick(Entity *e);

#endif