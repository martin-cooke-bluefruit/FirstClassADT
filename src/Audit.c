#include "Audit.h"

#include <stdio.h>

void Audit_LogEvent(const char* message)
{
  printf("Audit Event:\r\n  %s\r\n", message);
}
