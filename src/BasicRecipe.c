#include "BasicRecipe.h"
#include <stdio.h>

void BasicRecipe_PrintInfo(BasicRecipe* recipe)
{
  printf("\r\n");
  printf("Recipe Info:\r\n");
  printf("  Name:        %s\r\n", recipe->name);
  printf("  Volume:      %0.3f ml\r\n", recipe->voulmeInMicroliters / 1000.0);
  printf("  Start delay: %u ms\r\n", recipe->startDelayInMs);
  printf("\r\n");
}
