#include "Recipe.h"

#include <string.h>
#include <stdio.h>

#include "Audit.h"

#define MAX_NAME_LENGTH 30
#define MAX_START_DELAY 60000

struct Recipe
{
  char name[MAX_NAME_LENGTH + 1];
  uint32_t voulmeInMicroliters;
  uint32_t startDelayInMs;
};

#define MAX_RECIPIES 100
struct Recipe mRecipeStorage[MAX_RECIPIES];
RecipePtr mRecipePointers[MAX_RECIPIES];
int mNextUnusedRecipe;

char mAuditMessage[256];
const char* mErrorText;
const char* mNoError = "";

void Recipe_SetToDefault(RecipePtr recipe)
{
  strcpy(recipe->name, "Default encapsulated recipe");
  recipe->voulmeInMicroliters = 1234;
  recipe->startDelayInMs = 5678;
}

void Recipe_PrintInfo(RecipePtr recipe)
{
  printf("\r\n");
  printf("Recipe Info:\r\n");
  printf("  Name:        %s\r\n", recipe->name);
  printf("  Volume:      %0.3f ml\r\n", recipe->voulmeInMicroliters / 1000.0);
  printf("  Start delay: %u ms\r\n", recipe->startDelayInMs);
  printf("\r\n");
}

char* Recipe_GetName(RecipePtr recipe)
{
  return recipe->name;
}

bool Recipe_SetName(RecipePtr recipe, char* name)
{
  if (strlen(name) > MAX_NAME_LENGTH)
  {
    mErrorText = "New name is too long.";
    return false;
  }

  sprintf(mAuditMessage, "Recipe name for \"%s\" changed to \"%s\"",
    recipe->name,
    name);
  Audit_LogEvent(mAuditMessage);

  strcpy(recipe->name, name);

  mErrorText = mNoError;
  return true;
}

uint32_t Recipe_GetVoulme(RecipePtr recipe)
{
  return recipe->voulmeInMicroliters;
}

bool Recipe_SetVolume(RecipePtr recipe, uint32_t volume)
{
  recipe->voulmeInMicroliters = volume;

  sprintf(mAuditMessage, "Recipe \"%s\" volume set to %i microliters",
    recipe->name,
    recipe->voulmeInMicroliters);
  Audit_LogEvent(mAuditMessage);

  mErrorText = mNoError;
  return true;
}

uint32_t Recipe_GetStartDelay(RecipePtr recipe)
{
  return recipe->startDelayInMs;
}

bool Recipe_SetStartDelay(RecipePtr recipe, uint32_t startDelay)
{
  if (startDelay > MAX_START_DELAY)
  {
    mErrorText = "New start delay exceeds maximum limit.";
    return false;
  }

  recipe->startDelayInMs = startDelay;

  sprintf(mAuditMessage, "Recipe \"%s\" start delay set to %i milliseconds",
    recipe->name,
    recipe->startDelayInMs);
  Audit_LogEvent(mAuditMessage);

  mErrorText = mNoError;
  return true;
}

const char* Recipe_GetLastError(void)
{
  return mErrorText;
}

void Recipe_Init(void)
{
  mErrorText = mNoError;

  // Create memory pool for recipies
  for (int i = 0; i < MAX_RECIPIES; i++)
  {
    mRecipePointers[i] = &mRecipeStorage[i];
  }
  mNextUnusedRecipe = 0;
}

RecipePtr Recipe_Create(void)
{
  if (mNextUnusedRecipe >= MAX_RECIPIES)
    return 0;

  RecipePtr recipe = mRecipePointers[mNextUnusedRecipe];
  Recipe_SetToDefault(recipe);
  mNextUnusedRecipe++;
  return recipe;
}

bool Recipe_Destroy(RecipePtr recipe)
{
  if (mNextUnusedRecipe <= 0)
    return false;

  mNextUnusedRecipe--;
  mRecipePointers[mNextUnusedRecipe] = recipe;
  return true;
}
