#ifndef RECIPE_H
#define RECIPE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Recipe* RecipePtr;

void Recipe_SetToDefault(RecipePtr recipe);
void Recipe_PrintInfo(RecipePtr recipe);

char* Recipe_GetName(RecipePtr recipe);
bool Recipe_SetName(RecipePtr recipe, char* name);

uint32_t Recipe_GetVoulme(RecipePtr recipe);
bool Recipe_SetVolume(RecipePtr recipe, uint32_t volume);

uint32_t Recipe_GetStartDelay(RecipePtr recipe);
bool Recipe_SetStartDelay(RecipePtr recipe, uint32_t startDelay);

const char* Recipe_GetLastError(void);

// Recipe memory management
void Recipe_Init(void);
RecipePtr Recipe_Create(void);
bool Recipe_Destroy(RecipePtr recipe);

#endif