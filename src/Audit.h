#ifndef AUDIT_H
#define AUDIT_H

#include <stdint.h>

void Audit_RecipeNameChange(char* recipeName, char* newValue);
void Audit_RecipeVolumeChange(char* recipeName, uint32_t oldValue, uint32_t newValue);
void Audit_RecipeStartDelayChange(char* recipeName, uint32_t oldValue, uint32_t newValue);
void Audit_PrintLog(void);
void Audit_IntToString(int value, char* string);

#endif