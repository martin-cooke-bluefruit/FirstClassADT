#ifndef BASIC_RECIPE_H
#define BASIC_RECIPE_H

#include <stdint.h>

typedef struct
{
  char name[32];
  uint32_t voulmeInMicroliters;
  uint32_t startDelayInMs;
} BasicRecipe;

void BasicRecipe_SetToDefault(BasicRecipe* recipe);
void BasicRecipe_PrintInfo(BasicRecipe* recipe);

#endif