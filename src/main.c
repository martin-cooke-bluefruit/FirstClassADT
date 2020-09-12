#include "Audit.h"
#include "BasicRecipeDemo.h"
#include "Recipe.h"
#include "RecipeDemo.h"
#include <stdio.h>

int main()
{
  printf("--== Basic recipe demo ==--\r\n");
  RecipeDemo_Basic();
  Audit_PrintLog();
  (void)getchar();

  Recipe_Init();
  printf("--== Encapsulated recipe demo ==--\r\n");
  RecipeDemo_Encapsulated();
  Audit_PrintLog();
  (void)getchar();

  return 0;
}
