#include "Recipe.h"
#include "RecipeDemo.h"
#include <stdio.h>

int main ()
{
  printf("--== Basic recipe demo ==--\r\n");
  RecipeDemo_Basic();

  Recipe_Init();
  printf("--== Encapsulated recipe demo ==--\r\n");
  RecipeDemo_Encapsulated();

  return 0;
}

