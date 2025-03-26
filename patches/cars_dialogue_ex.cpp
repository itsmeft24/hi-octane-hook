
#include "patches/cars_dialogue_ex.hpp"
#include "sunset/sunset.hpp"
#include "core/logging.hpp"

namespace legacy {
	inline void inline_replace(void* src, void* dst, std::size_t size) {
		sunset::inst::nop(src, size);
		sunset::inst::call(src, dst);
	}
};

constexpr unsigned char kMaxNumberOfCars = 127;

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
		cmp[ebx + ecx + 0xA44], 0x0
		ret
	}
}

__declspec(naked) void CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH() {
	__asm {
		// lea ebp, [ebx + ecx * 0x4 + 0x114]
		lea ebp, [ebx + ecx + 0xA40]
		ret
	}
}

__declspec(naked) void CarsDialogue_UNK_LEA_PATCH() {
	__asm {
		// lea ebx, [edi + 0x11C]
		lea ebx, [edi + 0xA48]
		ret
	}
}

__declspec(naked) void CarsDialogue_UNK2_LEA_PATCH() {
	__asm {
		// lea edi, [ebx + 0x11C]
		lea edi, [ebx + 0xA48]
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

__declspec(naked) void CarsDialogue_UNK7_LEA_PATCH() {
	__asm {
		// lea edi, [ebx + 0x114]
		lea edi, [ebx + 0xA40]
		ret
	}
}

__declspec(naked) void CarsDialogue_UNK8_LEA_PATCH() {
	__asm {
		// lea edi, [ebx + 0x118]
		lea edi, [ebx + 0xA44]
		ret
	}
}

void cars_dialogue_ex::install() {
	logging::log("[cars_dialogue_ex::install] Patching CarsDialogue class...");

	sunset::utils::set_permission(reinterpret_cast<void*>(0x00482B6F), 1, sunset::utils::Perm::ExecuteReadWrite);
	sunset::utils::set_permission(reinterpret_cast<void*>(0x00482CCD), 1, sunset::utils::Perm::ExecuteReadWrite);
	sunset::utils::set_permission(reinterpret_cast<void*>(0x004C1FDE), 1, sunset::utils::Perm::ExecuteReadWrite);
	sunset::utils::set_permission(reinterpret_cast<void*>(0x004C1FE3), 1, sunset::utils::Perm::ExecuteReadWrite);

	*(unsigned char*)(0x00482B6F) = kMaxNumberOfCars;
	*(unsigned char*)(0x00482CCD) = kMaxNumberOfCars;
	*(unsigned char*)(0x004C1FDE) = kMaxNumberOfCars;
	*(unsigned char*)(0x004C1FE3) = kMaxNumberOfCars;

	sunset::inst::push_u32(reinterpret_cast<void*>(0x004F32C8), 0xA40 + 0x1C * kMaxNumberOfCars); // patch class size

	legacy::inline_replace(reinterpret_cast<void*>(0x004C2965), &CarsDialogue_BumpWreckOutOfControl_LEA_PATCH, 0x11); // PlayBumpDialogue
	legacy::inline_replace(reinterpret_cast<void*>(0x004C2977), &CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH, 0x7); // PlayBumpDialogue

	legacy::inline_replace(reinterpret_cast<void*>(0x004C2A95), &CarsDialogue_BumpWreckOutOfControl_LEA_PATCH, 0x11); // PlayWreckDialogue
	legacy::inline_replace(reinterpret_cast<void*>(0x004C2AA7), &CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH, 0x7); // PlayWreckDialogue

	legacy::inline_replace(reinterpret_cast<void*>(0x004C2C95), &CarsDialogue_BumpWreckOutOfControl_LEA_PATCH, 0x11); // PlayOutOfControlDialogue
	legacy::inline_replace(reinterpret_cast<void*>(0x004C2CA7), &CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH, 0x7); // PlayOutOfControlDialogue

	legacy::inline_replace(reinterpret_cast<void*>(0x00482B72), &CarsDialogue_CarsDialogue_LEA_PATCH, 0x6); // CarsDialogue
	legacy::inline_replace(reinterpret_cast<void*>(0x00482CD0), &CarsDialogue_CarsDialogue_LEA_PATCH, 0x6); // ~CarsDialogue

	legacy::inline_replace(reinterpret_cast<void*>(0x0043E251), &CarsDialogue_UNK_LEA_PATCH, 0x6); // UNK

	legacy::inline_replace(reinterpret_cast<void*>(0x004C2161), &CarsDialogue_UNK2_LEA_PATCH, 0x6); // UNK2

	legacy::inline_replace(reinterpret_cast<void*>(0x004C1FF1), &CarsDialogue_Create_LEA_PATCH, 0x6); // Create

	legacy::inline_replace(reinterpret_cast<void*>(0x004C2194), &CarsDialogue_UNK3_LEA_PATCH, 16); // UNK3

	legacy::inline_replace(reinterpret_cast<void*>(0x00482D61), &CarsDialogue_LoadDialogue_LEA_PATCH, 16); // LoadDialogue
	legacy::inline_replace(reinterpret_cast<void*>(0x00482EA8), &CarsDialogue_Reset_LEA_PATCH, 0x6); // Reset

	legacy::inline_replace(reinterpret_cast<void*>(0x0048319E), &CarsDialogue_UNK4_LEA_PATCH, 16); // UNK4
	legacy::inline_replace(reinterpret_cast<void*>(0x004C2BB3), &CarsDialogue_UNK4_LEA_PATCH, 16); // PlayOuchDialogue
	legacy::inline_replace(reinterpret_cast<void*>(0x004C2F0D), &CarsDialogue_UNK5_LEA_PATCH, 16); // PlayPassedDialogue
	legacy::inline_replace(reinterpret_cast<void*>(0x004C3026), &CarsDialogue_UNK5_LEA_PATCH, 16); // UNK5

	legacy::inline_replace(reinterpret_cast<void*>(0x004C310E), &CarsDialogue_UNK4_LEA_PATCH, 16); // UNK6
	legacy::inline_replace(reinterpret_cast<void*>(0x004EECBB), &CarsDialogue_UNK4_LEA_PATCH, 16); // UNK7
	legacy::inline_replace(reinterpret_cast<void*>(0x004EED4A), &CarsDialogue_UNK4_LEA_PATCH, 16); // UNK8

	legacy::inline_replace(reinterpret_cast<void*>(0x004EEF15), &CarsDialogue_UNK6_LEA_PATCH, 16); // UNK9

	legacy::inline_replace(reinterpret_cast<void*>(0x004F4C51), &CarsDialogue_UNK4_LEA_PATCH, 16); // UNK10

	legacy::inline_replace(reinterpret_cast<void*>(0x004F4CAC), &CarsDialogue_UNK4_LEA_PATCH, 16); // UNK11

	legacy::inline_replace(reinterpret_cast<void*>(0x004C2DCD), &CarsDialogue_UNK5_LEA_PATCH, 16); // PlayPassDialogue

	legacy::inline_replace(reinterpret_cast<void*>(0x004E9EE3), &CarsDialogue_UNK7_LEA_PATCH, 6); // UNK12
	legacy::inline_replace(reinterpret_cast<void*>(0x004E9D6B), &CarsDialogue_UNK7_LEA_PATCH, 6); // UNK13
	legacy::inline_replace(reinterpret_cast<void*>(0x0043E06C), &CarsDialogue_UNK8_LEA_PATCH, 6); // UNK14

	logging::log("[cars_dialogue_ex::install] Succesfully moved CarsCharacterDialogue array!");
}
