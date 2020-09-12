#include "RecipeDemo.h"
#include "Recipe.h"
#include <stdio.h>
#include <string.h>

static void PrintError(void);

void RecipeDemo_Encapsulated(void)
{
  RecipePtr recipe = Recipe_Create();

  // Assign some data:
  if (!Recipe_SetName(recipe, "Modified recipe"))
    PrintError();
  if (!Recipe_SetVolume(recipe, 10000))
    PrintError();
  if (!Recipe_SetStartDelay(recipe, 300))
    PrintError();

  // Read some data:
  printf("\r\nValid recipe data has been set: {\"%s\", %i, %i}\r\n",
         Recipe_GetName(recipe),
         Recipe_GetVoulme(recipe),
         Recipe_GetStartDelay(recipe));

  // Introduce some errors:
  printf("\r\nAttempting to assign some invalid data...\r\n");
  if (!Recipe_SetName(recipe, "Recipe name is too long 1234567890"))
    PrintError();
  if (!Recipe_SetStartDelay(recipe, -300))
    PrintError();

  // Show the results:
  Recipe_PrintInfo(recipe);

  Recipe_Destroy(recipe);
}

static void PrintError(void)
{
  printf("*** Error: %s ***\r\n", Recipe_GetLastError());
}
