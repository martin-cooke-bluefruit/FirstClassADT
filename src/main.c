#include <stdio.h>
#include <string.h>
#include "Audit.h"
#include "BasicRecipe.h"
#include "Recipe.h"

void BasicRecipeDemo(void);
void EncapsulatedRecipeDemo(void);
void PrintError(void);

int main ()
{
  printf("--== Basic recipe demo ==--\r\n");
  BasicRecipeDemo();

  Recipe_Init();
  printf("--== Encapsulated recipe demo ==--\r\n");
  EncapsulatedRecipeDemo();

  return 0;
}

void BasicRecipeDemo(void)
{
  BasicRecipe recipe;

  // With the basic recipe, unchecked changes are possible from anywhere in the codebase.
  // As such, any limit checking, auditing, etc. is left up to every coder who uses the module.
  
  // If we are careful with how we use the BasicRecipe structure then things work as expected.
  // However, should we generate audit events for our changes, or not?
  strcpy(recipe.name, "Modified recipe");
  recipe.voulmeInMicroliters = 10000;
  recipe.startDelayInMs = 300;

  // If we aren't so careful with with our BasicRecipe struct then we can run into problems:

  // Blindly copying bytes to a pointer we don't own is risky!
  // e.g, This strcpy overwrites the recipe volume with bytes representing "90!\0" (or 0x39302100).
  // This equates to the decimal value 2175033 (on a little-endian system), hence 2175.033 ml
  strcpy(recipe.name, "Recipe name is too long 1234567890!");

  // A typo? -300ms stored as uint32_t is about 50 days!!
  recipe.startDelayInMs = -300;

  // The recipe is changed, but the errors were not handled ...
  BasicRecipe_PrintInfo(&recipe);

  // ... and nothing was audited
  Audit_PrintLog();
}

void EncapsulatedRecipeDemo(void)
{
  RecipePtr recipe = Recipe_Create();

  // Unchecked changes to recipe data are no longer allowed.
  //recipe->voulmeInMicroliters = 1234;  // Compile error, dereferencing not allowed

  // With the encapsulated recipe, data modification can only occur via the recipe module istelf.
  // As such, limit checking, auditing, etc. take place in one central location.

  // The following three changes are valid so no error will be printed,
  // and creation of audit events will be handled by the recipe module:
  if (!Recipe_SetName(recipe, "Modified recipe"))
    PrintError();

  if (!Recipe_SetVolume(recipe, 10000))
    PrintError();

  if (!Recipe_SetStartDelay(recipe, 300))
    PrintError();

  // The next two examples fail (i.e. return false). As such the recipe data is unchanged
  // and we can get the error details from the recipe module to print to the console:

  // This text is too long so the change will be rejected:
  if (!Recipe_SetName(recipe, "Recipe name is too long 1234567890!"))
    PrintError();

  // -300 as an uint32_t would be parsed as 4294966996, so it gets rejected as too large:
  if (!Recipe_SetStartDelay(recipe, -300))
    PrintError();

  // The errors were handled and the recipe is still correct
  Recipe_PrintInfo(recipe);

  // The audit log contains a record of recipe changes
  Audit_PrintLog();

  // Optionally, we can free up memory if we no longer need the recipe
  Recipe_Destroy(recipe);
}

void PrintError(void)
{
  printf("*** Error: %s ***\r\n", Recipe_GetLastError());
}
