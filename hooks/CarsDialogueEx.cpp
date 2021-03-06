
#include "CarsDialogueEx.h"
#include "../HookFunction.h"
#include "../Logging.h"
#include "../Utils.h"

__declspec(naked) void CarsDialogue_CarsDialogue_LEA_PATCH() {
  __asm {
    // lea eax, [esi + 0x114]
		lea eax, [esi + 0xA40]
		ret
  }
}

__declspec(naked) void CarsDialogue_Create_LEA_PATCH() {
  __asm {
    // lea edi, [esi + 0x120]
		lea edi, [esi + 0xA4C]
		ret
  }
}

__declspec(naked) void CarsDialogue_LoadDialogue_LEA_PATCH() {
  __asm {
    // lea ecx, [eax * 0x8]
    // sub ecx, eax
    // lea esi, [edi + ecx * 4 + 0x114]
		imul ecx, eax, 0x1C
		lea esi, [edi + ecx + 0xA40]
		ret
  }
}

__declspec(naked) void CarsDialogue_Reset_LEA_PATCH() {
  __asm {
    // lea ebx, [esi + 0x114]
		lea ebx, [esi + 0xA40]
		ret
  }
}

__declspec(naked) void CarsDialogue_BumpWreckOutOfControl_LEA_PATCH() {
  __asm {
    // lea ecx, [eax * 0x8]
    // sub ecx, eax
    // cmp [ebx + ecx * 0x4 + 0x118], 0x0
		imul ecx, eax, 0x1C
		cmp [ebx + ecx + 0xA44], 0x0
		ret
  }
}

__declspec(naked) void CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH() {
  __asm {
    // lea ebp, [ebx + ecx * 0x4 + 0x114]
		lea ebp, [ebx + ecx + 0xA44]
		ret
  }
}

__declspec(naked) void CarsDialogue_UNK_LEA_PATCH() {
  __asm {
    // lea eax, [edi + 0x11C]
		lea ebx, [edi + 0xA48]
		ret
  }
}

__declspec(naked) void CarsDialogue_UNK2_LEA_PATCH() {
  __asm {
    // lea edi, [esi + 0x11C]
		lea edi, [esi + 0xA48]
		ret
  }
}

__declspec(naked) void CarsDialogue_UNK3_LEA_PATCH() {
  __asm {
    // lea ecx, [esi * 0x8]
    // sub ecx, esi
    // lea esi, [ebx + ecx * 4 + 0x114]
		imul ecx, esi, 0x1C
		lea esi, [ebx + ecx + 0xA40]
		ret
  }
}

__declspec(naked) void CarsDialogue_UNK4_LEA_PATCH() {
  __asm {
    // lea ecx, [eax * 0x8]
    // sub ecx, eax
    // lea eax, [esi + ecx * 4 + 0x114]
		imul ecx, eax, 0x1C
		lea eax, [esi + ecx + 0xA40]
		ret
  }
}

__declspec(naked) void CarsDialogue_UNK5_LEA_PATCH() {
  __asm {
    // lea ecx, [eax * 0x8]
    // sub ecx, eax
    // lea eax, [edi + ecx * 4 + 0x114]
		imul ecx, eax, 0x1C
		lea eax, [edi + ecx + 0xA40]
		ret
  }
}

__declspec(naked) void CarsDialogue_UNK6_LEA_PATCH() {
  __asm {
    // lea ecx, [eax * 0x8]
    // sub ecx, eax
    // lea eax, [ebx + ecx * 4 + 0x114]
		imul ecx, eax, 0x1C
		lea eax, [ebx + ecx + 0xA40]
		ret
  }
}

void CarsDialogueEx::Install() {
  Logging::Log("[CarsDialogueEx] Patching CarsDialogue class...\n");
  WritePUSH(AsVoidPtr(0x004f32c8),
            AsVoidPtr(0xA40 + 0x1BE4)); // patch class size

  HookFunction(0x004C2965, &CarsDialogue_BumpWreckOutOfControl_LEA_PATCH, 0x11,
               FunctionHookType::InlineReplacement); // PlayBumpDialogue
  HookFunction(0x004c2977, &CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH,
               0x7, FunctionHookType::InlineReplacement); // PlayBumpDialogue

  HookFunction(0x004C2A95, &CarsDialogue_BumpWreckOutOfControl_LEA_PATCH, 0x11,
               FunctionHookType::InlineReplacement); // PlayWreckDialogue
  HookFunction(0x004C2AA7, &CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH,
               0x7, FunctionHookType::InlineReplacement); // PlayWreckDialogue

  HookFunction(0x004C2C95, &CarsDialogue_BumpWreckOutOfControl_LEA_PATCH, 0x11,
               FunctionHookType::InlineReplacement); // PlayOutOfControlDialogue
  HookFunction(0x004C2CA7, &CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH,
               0x7,
               FunctionHookType::InlineReplacement); // PlayOutOfControlDialogue

  HookFunction(0x00482B72, &CarsDialogue_CarsDialogue_LEA_PATCH, 0x6,
               FunctionHookType::InlineReplacement); // CarsDialogue
  HookFunction(0x00482CD0, &CarsDialogue_CarsDialogue_LEA_PATCH, 0x6,
               FunctionHookType::InlineReplacement); // ~CarsDialogue

  HookFunction(0x0043E251, &CarsDialogue_UNK_LEA_PATCH, 0x6,
               FunctionHookType::InlineReplacement); // UNK

  HookFunction(0x004C2161, &CarsDialogue_UNK2_LEA_PATCH, 0x6,
               FunctionHookType::InlineReplacement); // UNK2

  HookFunction(0x004C1FF1, &CarsDialogue_Create_LEA_PATCH, 0x6,
               FunctionHookType::InlineReplacement); // Create

  HookFunction(0x004C2194, &CarsDialogue_UNK3_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // UNK3

  HookFunction(0x00482D61, &CarsDialogue_LoadDialogue_LEA_PATCH, 0x7,
               FunctionHookType::InlineReplacement); // LoadDialogue
  HookFunction(0x00482EA8, &CarsDialogue_Reset_LEA_PATCH, 0x6,
               FunctionHookType::InlineReplacement); // Reset

  HookFunction(0x0048319E, &CarsDialogue_UNK4_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // UNK4
  HookFunction(0x004C2BB3, &CarsDialogue_UNK4_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // PlayOuchDialogue
  HookFunction(0x004C2BB3, &CarsDialogue_UNK5_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // PlayPassedDialogue
  HookFunction(0x004C3026, &CarsDialogue_UNK5_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // UNK5

  HookFunction(0x004C310E, &CarsDialogue_UNK4_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // UNK6
  HookFunction(0x004EECBB, &CarsDialogue_UNK4_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // UNK7
  HookFunction(0x004EED4A, &CarsDialogue_UNK4_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // UNK8

  HookFunction(0x004EEF15, &CarsDialogue_UNK6_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // UNK9

  HookFunction(0x004F4C51, &CarsDialogue_UNK4_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // UNK10

  HookFunction(0x004F4CAC, &CarsDialogue_UNK4_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // UNK11

  HookFunction(0x004C2DCD, &CarsDialogue_UNK5_LEA_PATCH, 16,
               FunctionHookType::InlineReplacement); // PlayPassDialogue

  Logging::Log("[CarsDialogueEX::Install] Succesfully moved "
               "CarsCharacterDialogue array!\n");

  auto lpCarsDialogue =
      *(DWORD *)(*(DWORD *)(0x00718A74 + 1088) +
                 68); // lpCarsGame->lpCarsAudioManager->lpCarsDialogue
}
