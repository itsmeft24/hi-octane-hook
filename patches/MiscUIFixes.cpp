
#include "MiscUIFixes.h"
#include "../framework.hpp"
#include "../Logging.h"

DeclareFunction(int, __thiscall, CarsUIManager_PlaySoundEvent, 0x005c9760, uintptr_t, int, int, int);

DefineReplacementHook(PlaySelectAndBackSnds) {
	static void __fastcall callback(uintptr_t layer) {

	  // Call original function.
	  original(layer);

	  // Find pCarsUIManager instance.
	  uintptr_t UnkStruct1 = *(uintptr_t *)(0x006FE590);
	  uintptr_t UnkStruct2 = *(uintptr_t *)(UnkStruct1 + 0x24);
	  uintptr_t pCarsUIManager = *(uintptr_t *)(UnkStruct2 + 0xF0);

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
		CarsUIManager_PlaySoundEvent(pCarsUIManager, SoundEffectIndex, 1, 1);
	  }
	}
};

void fix_ui_sounds::install() {
	PlaySelectAndBackSnds::install_at_ptr(0x00511e00);
    logging::log("[MiscUIFixes::Install] Successfully installed patch!");
}