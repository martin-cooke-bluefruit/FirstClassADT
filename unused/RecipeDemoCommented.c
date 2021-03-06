#include "Audit.h"
#include "BasicRecipe.h"
#include "Recipe.h"
#include <stdio.h>
#include <string.h>

static void PrintError(void);

static void BasicDemoCommented(void)
{
  BasicRecipe recipe;

  // With the basic recipe, unchecked changes are possible from anywhere in the codebase.
  // As such, any limit checking, auditing, etc. is left up to every coder who uses the module.

  // If we are careful with how we use the BasicRecipe structure then things work as expected.
  // However, should we generate audit events for our changes, or not?
  strcpy(recipe.name, "Modified recipe");
  recipe.voulmeInMicroliters = 10000;
  recipe.startDelayInMs = 300;

  // We can access the data directly
  printf("Valid recipe data has been set: {\"%s\", %i, %i}\r\n",
         recipe.name,
         recipe.voulmeInMicroliters,
         recipe.startDelayInMs);

  printf("Attempting to assign some invalid data...\r\n");
  // If we aren't so careful with with our BasicRecipe struct then we can run into problems:

  // Blindly copying bytes to a pointer we don't own is risky!
  // e.g, This strcpy overwrites the recipe volume with bytes representing "90\0" (or 0x393000).
  // This equates to the decimal value 12345 (on a little-endian system), hence 12.345 ml
  strcpy(recipe.name, "Recipe name is too long 1234567890");

  // A typo? -300ms stored as uint32_t is about 50 days!!
  recipe.startDelayInMs = -300;

  // The recipe is changed, but the errors were not handled ...
  BasicRecipe_PrintInfo(&recipe);
}

static void EncapsulatedDemoCommented(void)
{
  RecipePtr recipe = Recipe_Create();

  // With the encapsulated recipe, data modification can only occur via the recipe module istelf.
  // As such, limit checking, auditing, etc. take place in one central location.

  // Direct access to to recipe data is no longer allowed. Uncomment and try to compile:
  //recipe->voulmeInMicroliters = 1234;  // Compile error, dereferencing not allowed

  // The following changes are not direct assignmets but are instead via Recipe_SetXYZ() accessors.
  // The data being set here is valid so no error will be printed, and creation of audit events
  // will be handled by the recipe module:
  if (!Recipe_SetName(recipe, "Modified recipe"))
    PrintError();

  if (!Recipe_SetVolume(recipe, 10000))
    PrintError();

  if (!Recipe_SetStartDelay(recipe, 300))
    PrintError();

  // Similarly, we must use accessors to get data instead of directly reading from the data struct.
  printf("Valid recipe data has been set: {\"%s\", %i, %i}\r\n",
         Recipe_GetName(recipe),
         Recipe_GetVoulme(recipe),
         Recipe_GetStartDelay(recipe));

  printf("Attempting to assign some invalid data...\r\n");
  // The next two examples fail (i.e. return false). As such the recipe data is unchanged
  // and we can get the error details from the recipe module to print to the console:

  // This text is too long so the change will be rejected:
  if (!Recipe_SetName(recipe, "Recipe name is too long 1234567890"))
    PrintError();

  // -300 as an uint32_t would be parsed as 4294966996, so it gets rejected as too large:
  if (!Recipe_SetStartDelay(recipe, -300))
    PrintError();

  // The errors were handled and the recipe is still correct
  Recipe_PrintInfo(recipe);

  // Optionally, we can free up memory if we no longer need the recipe
  Recipe_Destroy(recipe);
}

static void PrintError(void)
{
  printf("*** Error: %s ***\r\n", Recipe_GetLastError());
}
