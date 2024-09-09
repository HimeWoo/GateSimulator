#include "entityTypes.h"

EntityType GateNOT;
EntityType GateOR;
EntityType GateAND;

void InitEntityTypes(void)
{
  Rectangle outline = CLITERAL(Rectangle){0, 0, 128, 128};

  GateNOT.defaultNumIn = 1;
  GateNOT.defaultNumOut = 1;
  Image NOTImage = GenImageColor(128, 128, BLANK);
  ImageDrawRectangleLines(&NOTImage, outline, 8.0f, WHITE);
  ImageDrawText(&NOTImage, "NOT", 16, 16, 48, WHITE);
  GateNOT.texture = LoadTextureFromImage(NOTImage);
  //GateNOT.Operator = &opNOT;
  UnloadImage(NOTImage);

  GateOR.defaultNumIn = 2;
  GateOR.defaultNumOut = 1;
  Image ORImage = GenImageColor(128, 128, BLANK);
  ImageDrawRectangleLines(&ORImage, outline, 8.0f, WHITE);
  ImageDrawText(&ORImage, "OR", 16, 16, 48, WHITE);
  GateOR.texture = LoadTextureFromImage(ORImage);
  //GateOR.Operator = &opOR;
  UnloadImage(ORImage);

  GateAND.defaultNumIn = 2;
  GateAND.defaultNumOut = 1;
  Image ANDImage = GenImageColor(128, 128, BLANK);
  ImageDrawRectangleLines(&ANDImage, outline, 8.0f, WHITE);
  ImageDrawText(&ANDImage, "AND", 16, 16, 48, WHITE);
  GateAND.texture = LoadTextureFromImage(ANDImage);
  //GateAND.Operator = &opAND;
  UnloadImage(ANDImage);
}