
#include <Windows.h>

#include "../Logging.h"
#include "../Utils.h"

#include "AutomaticBoostPatch.h"

DWORD *pBoostLevel = (DWORD *)0x006ED818;

void AutomaticBoostPatch::Install() {
  SetReadWritePermission(pBoostLevel, 4);
  *pBoostLevel = 3;
  Logging::Log(
      "[AutomaticBoostPatch::Install] Successfully installed patch!\n");
}