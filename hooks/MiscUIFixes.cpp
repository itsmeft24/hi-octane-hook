
#include "MiscUIFixes.h"
#include "../HookFunction.h"
#include "../Logging.h"
#include "../Utils.h"

DeclareFunction(int, __thiscall, CarsUIManager_PlaySoundEvent, 0x005c9760,
                uintptr_t, int, int, int);
DeclareFunction(void, __thiscall, UnkLayer_HandleOK, 0x00511e00, uintptr_t);

HookedFunctionInfo unklayer_handleok_finfo;

void __fastcall UnkLayer_HandleOK_Hook(uintptr_t layer) {

  // Call original function.
  UnkLayer_HandleOK(layer);

  // Find pCarsUIManager instance.
  uintptr_t UnkStruct1 = *(uintptr_t *)(0x006FE590);
  uintptr_t UnkStruct2 = *(uintptr_t *)(UnkStruct1 + 0x24);
  uintptr_t pCarsAudioManager = *(uintptr_t *)(UnkStruct2 + 0xF0);

  int SoundEffectIndex = 0;

  // Find and trigger the correct sound event.
  uintptr_t UnkStruct3 = *(int *)(layer + 0x28);
  if (UnkStruct3 != 0) {
    switch (*(unsigned int *)(UnkStruct3 + 0x8C)) {
    case 1:
    case 10:
    case 0xb:
      if (*(int *)(UnkStruct3 + 0xb0) != 0) {
        return;
      }
      SoundEffectIndex = 4;
      break;
    case 2:
    case 8:
      if (*(int *)(UnkStruct3 + 0xb0) != 0) {
        return;
      }
      SoundEffectIndex = 3;
      break;
    default:
      return;
    }
    *(int *)(UnkStruct3 + 0xb0) = 1;
    CarsUIManager_PlaySoundEvent(pCarsAudioManager, SoundEffectIndex, 1, 1);
  }
}

void MiscUIFixes::Install() {
  unklayer_handleok_finfo =
      HookFunction((void *&)UnkLayer_HandleOK, UnkLayer_HandleOK_Hook, 6,
                   FunctionHookType::EntireReplacement);
  if (unklayer_handleok_finfo.type != FunctionHookType::Invalid)
    Logging::Log("[MiscUIFixes::Install] Successfully installed patch!\n");
}