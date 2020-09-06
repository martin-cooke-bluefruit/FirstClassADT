#include <stdio.h>
#include <string.h>
#include "Audit.h"
#include "BasicRecipe.h"
#include "Recipe.h"

void BasicRecipeDemo(void);
void EncapsulatedRecipeDemo(void);

int main ()
{
  printf("--== Basic recipe demo ==--\r\n");
  BasicRecipeDemo();

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
  
  // Blindly copying bytes to a pointer we don't own is risky!
  strcpy(recipe.name, "Modified recipe");
  /* e.g, The following would overwrite unrelated memory and produce undefined behaviour:
  strcpy(recipe.name, "This new recipe name is too long for the buffer defined in recipe.c"); */
  
  // Should this generate an audit event, or not?
  recipe.voulmeInMicroliters = 10000;

  // A typo? -300ms stored as uint32_t is about 50 days!!
  recipe.startDelayInMs = -300;

  // The recipe is changed, but the errors were not handled ...
  BasicRecipe_PrintInfo(&recipe);

  // ... and nothing was audited
  Audit_PrintLog();
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

  Audit_PrintLog();

  // Optionally, free up memory if we no longer need the recipe
  Recipe_Destroy(encapsulatedRecipe);
}
