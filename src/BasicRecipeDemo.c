#include "BasicRecipeDemo.h"
#include "BasicRecipe.h"
#include <stdio.h>
#include <string.h>

void RecipeDemo_Basic(void)
{
  BasicRecipe recipe;

  // Assign some data:
  strcpy(recipe.name, "Modified recipe");
  recipe.voulmeInMicroliters = 10000;
  recipe.startDelayInMs = 300;

  // Read some data:
  printf("\r\nValid recipe data has been set: {\"%s\", %i, %i}\r\n",
         recipe.name,
         recipe.voulmeInMicroliters,
         recipe.startDelayInMs);

  // Introduce some errors:
  printf("\r\nAttempting to assign some invalid data...\r\n");
  strcpy(recipe.name, "Recipe name is too long 1234567890");
  recipe.startDelayInMs = -300;

  // Show the results:
  BasicRecipe_PrintInfo(&recipe);
}
