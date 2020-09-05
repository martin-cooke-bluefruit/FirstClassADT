#include <stdio.h>
#include <string.h>
#include "BasicRecipe.h"
#include "Recipe.h"

void BasicRecipeDemo(void);
void EncapsulatedRecipeDemo(void);

int main ()
{
  // printf("--== Basic recipe demo ==--\r\n");
  // BasicRecipeDemo();

  printf("--== Encapsulated recipe demo ==--\r\n");
  EncapsulatedRecipeDemo();

  return 0;
}

void BasicRecipeDemo(void)
{
  BasicRecipe recipe;
  BasicRecipe_SetToDefault(&recipe);
  BasicRecipe_PrintInfo(&recipe);

  // With the basic recipe, unchecked changes are possible from anywhere in the codebase.
  // As such, any limit checking, auditing, etc. is left up to every coder who uses the module.
  
  strcpy(recipe.name, "Modified recipe"); // strcpy with longer text could overwrite invalid memory!
  recipe.voulmeInMicroliters = 10000; // Should this generate an audit event, or not?
  recipe.startDelayInMs = -300;  // A typo? -300ms stored as uint32_t is about 50 days!!
  
  BasicRecipe_PrintInfo(&recipe);
}

void EncapsulatedRecipeDemo(void)
{
  Recipe_Init();

  RecipePtr encapsulatedRecipe = Recipe_Create();
  Recipe_PrintInfo(encapsulatedRecipe);

  // Unchecked changes to recipe data are no longer allowed.
  //encapsulatedRecipe->voulmeInMicroliters = 1234;  // Compile error, dereferencing not allowed

  // With the encapsulated recipe, data modification can only occur via the recipe module istelf.
  // As such, limit checking, auditing, etc. take place in one central location.

  // This text is too long so the change will be rejected:
  if (!Recipe_SetName(encapsulatedRecipe, "This text exceeds the maximum recipe name length!"))
    printf("*** Error: %s ***\r\n", Recipe_GetLastError());

  // -300 as an uint32_t would be parsed as 4294966996, so it gets rejected as too large:
  if (!Recipe_SetStartDelay(encapsulatedRecipe, -300))
    printf("*** Error: %s ***\r\n", Recipe_GetLastError());

  // The following three changes are valid so audit events will be created automatically:
  if (!Recipe_SetName(encapsulatedRecipe, "New recipe name"))
    printf("*** Error: %s ***\r\n", Recipe_GetLastError());

  if (!Recipe_SetVolume(encapsulatedRecipe, 7200))
    printf("*** Error: %s ***\r\n", Recipe_GetLastError());

  if (!Recipe_SetStartDelay(encapsulatedRecipe, 800))
    printf("*** Error: %s ***\r\n", Recipe_GetLastError());

  Recipe_PrintInfo(encapsulatedRecipe);

  Recipe_Destroy(encapsulatedRecipe);
}
