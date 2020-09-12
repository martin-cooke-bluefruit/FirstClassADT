#include "Recipe.h"
#include "Audit.h"
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 30
#define MAX_START_DELAY 60000

struct Recipe {
  char name[MAX_NAME_LENGTH + 1];
  uint32_t voulmeInMicroliters;
  uint32_t startDelayInMs;
};

#define MAX_RECIPIES 100
static struct Recipe mRecipeStorage[MAX_RECIPIES];
static RecipePtr mRecipePointers[MAX_RECIPIES];
static int mNextUnusedRecipe;

static const char* mErrorText;
static const char* mNoError = "";

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

  Audit_RecipeNameChange(recipe->name, name);

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
  Audit_RecipeVolumeChange(recipe->name, recipe->voulmeInMicroliters, volume);

  recipe->voulmeInMicroliters = volume;

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

  Audit_RecipeStartDelayChange(recipe->name, recipe->startDelayInMs, startDelay);

  recipe->startDelayInMs = startDelay;

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

  strcpy(recipe->name, "Default recipe");
  recipe->voulmeInMicroliters = 0;
  recipe->startDelayInMs = 0;

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
