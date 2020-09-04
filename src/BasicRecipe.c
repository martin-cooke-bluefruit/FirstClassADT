#include "BasicRecipe.h"

#include <string.h>
#include <stdio.h>

void BasicRecipe_SetToDefault(BasicRecipe* recipe)
{
  strcpy(recipe->name, "Default recipe");
  recipe->startDelayInMs = 1000;
  recipe->voulmeInMicroliters = 2500;
}

void BasicRecipe_PrintInfo(BasicRecipe* recipe)
{
  printf("\r\n");
  printf("Recipe Info:\r\n");
  printf("  Name:        %s\r\n", recipe->name);
  printf("  Volume:      %0.3f ml\r\n", recipe->voulmeInMicroliters / 1000.0);
  printf("  Start delay: %u ms\r\n", recipe->startDelayInMs);
  printf("\r\n");
}
