#include "entity.h"
#include <raymath.h>

int numEntities;
Entity **entities;

EntityType GateWire;
EntityType GateNOT;
EntityType GateOR;
EntityType GateAND;
EntityType IOSwitch;
EntityType IODisplay;

void InitEntityTypes(int tileSize)
{
  Rectangle outline = CLITERAL(Rectangle){0, 0, 128, 128};
  Image gateTemplate = GenImageColor(128, 128, BLANK);
  ImageDrawRectangleLines(&gateTemplate, outline, 8.0f, WHITE);
  Image image;

  GateNOT.numTextures = 1;
  GateNOT.textures = malloc(sizeof(Texture2D) * GateNOT.numTextures);
  image = ImageCopy(gateTemplate);
  ImageDrawText(&image, "NOT", 16, 16, 48, WHITE);
  GateNOT.textures[0] = LoadTextureFromImage(image);

  GateOR.numTextures = 1;
  GateOR.textures = malloc(sizeof(Texture2D) * GateOR.numTextures);
  image = ImageCopy(gateTemplate);
  ImageDrawText(&image, "OR", 16, 16, 48, WHITE);
  GateOR.textures[0] = LoadTextureFromImage(image);

  GateAND.numTextures = 1;
  GateAND.textures = malloc(sizeof(Texture2D) * GateAND.numTextures);
  image = ImageCopy(gateTemplate);
  ImageDrawText(&image, "AND", 16, 16, 48, WHITE);
  GateAND.textures[0] = LoadTextureFromImage(image);
  
  IOSwitch.numTextures = 2;
  IOSwitch.textures = malloc(sizeof(Texture2D) * IOSwitch.numTextures);
  image = ImageCopy(gateTemplate);
  ImageDrawCircle(&image, 64, 64, 32, DARKGREEN);
  IOSwitch.textures[0] = LoadTextureFromImage(image);
  image = ImageCopy(gateTemplate);
  ImageDrawCircle(&image, 64, 64, 32, GREEN);
  IOSwitch.textures[1] = LoadTextureFromImage(image);
  IOSwitch.onClick = &IOSwitch_onClick;

  IODisplay.textures = malloc(sizeof(Texture2D) * IODisplay.numTextures);
  IODisplay.numTextures = 1;
  image = ImageCopy(gateTemplate);
  IODisplay.textures[0] = LoadTextureFromImage(image);

  UnloadImage(gateTemplate);
  UnloadImage(image);
}

void InitEntities(void)
{
  entities = malloc(0);
  numEntities = 0;
}

Entity *NewEntity(EntityType t, Vector2 pos, Vector2 texOffset,
                  float texWidth, float texHeight, float rot)
{
  Entity *e = malloc(sizeof(Entity));
  e->position = pos;
  e->textureOffset = texOffset;
  e->width = texWidth;
  e->height = texHeight;
  e->type = t;
  e->rotation = rot;
  e->numIn = 0;
  e->in = malloc(0);
  e->numOut = 0;
  e->out = malloc(0);

  numEntities++;
  entities = realloc(entities, numEntities * sizeof(Entity));
  entities[numEntities - 1] = e;
  return e;
}

Rectangle GetTextureRec(Entity e)
{
  return CLITERAL(Rectangle){
      e.position.x - e.textureOffset.x,
      e.position.y - e.textureOffset.y,
      e.width,
      e.height};
}

void DrawEntity(Entity e, Color tint)
{
  Rectangle textureSrc = CLITERAL(Rectangle){
      0,
      0,
      e.type.textures[0].width,
      e.type.textures[0].height};
  Rectangle textureDest = CLITERAL(Rectangle){
      e.position.x,
      e.position.y,
      e.width,
      e.height};
  DrawTexturePro(e.type.textures[0], textureSrc, textureDest,
                 e.textureOffset, e.rotation, tint);
}

void ConnectEntities(Entity *output, Entity *input)
{
  output->out = realloc(output->out, output->numOut + 1);
  output->out[output->numOut] = input;
  output->numOut++;
  input->in = realloc(input->in , input->numIn + 1);
  input->in[input->numIn] = output;
  input->numIn++;
}

void PrintEntityInfo(Entity *e)
{
  printf("Entity: <%p> ---------------------------\n", e);
  printf("| entity pos:\t(%.2f, %.2f)\n", e->position.x, e->position.y);
  printf("| texture pos:\t(%.2f, %.2f)\n",
         e->position.x + e->textureOffset.x,
         e->position.y + e->textureOffset.y);
  printf("| dimensions:\t%.2f x %.2f\n", e->width, e->height);
  printf("| rotation:\t%.2f\n", e->rotation);
  printf("| inputs:\t%d\n", e->numIn);
  for (int i = 0; i < e->numIn; i++)
  {
    printf("| | %d. <%p>\n", i, e->in[i]);
  }
  printf("| outputs:\t%d\n", e->numIn);
  for (int i = 0; i < e->numOut; i++)
  {
    printf("| | %d. <%p>\n", i, e->out[i]);
  }
}

void IOSwitch_onClick(Entity *e)
{
  printf("Switch clicked\n");
}