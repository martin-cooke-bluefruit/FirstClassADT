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

  // With the basic recipe, unchecked changes are possible from anywhere in the codebase.
  // As such, any limit checking, auditing, etc. is left up to every coder who uses the module.
  

  // Should these changes generate audit events, or not?
  recipe.voulmeInMicroliters = 10000;

  // Blindly copying bytes to a pointer we don't own is risky!
  // e.g, This strcpy overwrites the recipe volume with bytes representing "90\0" (or 0x393000).
  // This equates to the decimal value 12345 (or 3747840 on a big-endian system)
  strcpy(recipe.name, "Recipe name is too long 1234567890");

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

  // Unchecked changes to recipe data are no longer allowed.
  //encapsulatedRecipe->voulmeInMicroliters = 1234;  // Compile error, dereferencing not allowed

  // With the encapsulated recipe, data modification can only occur via the recipe module istelf.
  // As such, limit checking, auditing, etc. take place in one central location.

  // This text is too long so the change will be rejected:
  if (!Recipe_SetName(encapsulatedRecipe, "Recipe name is too long 1234567890"))
    printf("*** Error: %s ***\r\n", Recipe_GetLastError());

  // -300 as an uint32_t would be parsed as 4294966996, so it gets rejected as too large:
  if (!Recipe_SetStartDelay(encapsulatedRecipe, -300))
    printf("*** Error: %s ***\r\n", Recipe_GetLastError());

  // The following three changes are valid so audit events will be created automatically:
  if (!Recipe_SetName(encapsulatedRecipe, "Modified recipe"))
    printf("*** Error: %s ***\r\n", Recipe_GetLastError());

  if (!Recipe_SetVolume(encapsulatedRecipe, 10000))
    printf("*** Error: %s ***\r\n", Recipe_GetLastError());

  if (!Recipe_SetStartDelay(encapsulatedRecipe, 300))
    printf("*** Error: %s ***\r\n", Recipe_GetLastError());

  Recipe_PrintInfo(encapsulatedRecipe);

  Audit_PrintLog();

  // Optionally, free up memory if we no longer need the recipe
  Recipe_Destroy(encapsulatedRecipe);
}
