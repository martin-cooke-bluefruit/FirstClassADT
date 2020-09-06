#include "Audit.h"

#include <stdio.h>
#include <string.h>

#define AUDIT_LOG_SIZE 1024

void AddMessageToLog(char* newAuditMessage);

char mAuditLog[AUDIT_LOG_SIZE];


void Audit_RecipeNameChange(char* recipeName, char* newName)
{
  char newAuditMessage[255];
  sprintf(newAuditMessage, "Recipe \"%s\": name changed to \"%s\".\r\n", recipeName, newName);
  AddMessageToLog(newAuditMessage);
}

void Audit_RecipeVolumeChange(char* recipeName, uint32_t oldMicroliters, uint32_t newMicroliters)
{
  char newAuditMessage[255];
  sprintf(
    newAuditMessage,
    "Recipe \"%s\": volume changed from %0.3f ml to %0.3f ml.\r\n",
    recipeName,
    oldMicroliters / 1000.0,
    newMicroliters / 1000.0);
  AddMessageToLog(newAuditMessage);
}

void Audit_RecipeStartDelayChange(char* recipeName, uint32_t oldMilliseconds, uint32_t newMilliseconds)
{
  char newAuditMessage[255];
  sprintf(
    newAuditMessage,
    "Recipe \"%s\": start delay changed from %i ms to %i ms.\r\n",
    recipeName,
    oldMilliseconds,
    newMilliseconds);
  AddMessageToLog(newAuditMessage);
}

void AddMessageToLog(char* newAuditMessage)
{
  int availableLogMemory = AUDIT_LOG_SIZE - strlen(mAuditLog);
  if (strlen(newAuditMessage) < availableLogMemory)
    strcat(mAuditLog, newAuditMessage);
  else
    printf("Warning: Insufficient log space for new audit message");
}

void Audit_PrintLog(void)
{
  printf("--AUDIT LOG START--\r\n%s--AUDIT LOG END--\r\n\r\n", mAuditLog);
}
