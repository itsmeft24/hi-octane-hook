#include "misc.hpp"

#include "core/hooking/framework.hpp"

struct BlockAllocator;

struct SurfaceShader {
	int next_idx;
	BlockAllocator* block_allocator;
};
struct X360SurfaceShaderList {
	void* vtable;
	int unk;
	int unk_buffer[0x1000];
	SurfaceShader surface_shaders[4];
};
static_assert(sizeof(X360SurfaceShaderList) == 16424);

/* Fixes an issue in SurfaceShaderList::ActivateMaterial where the game will try to access SurfaceShaders outside the surfaceShaders array by adding bounds checks. */
DefineReplacementHook(FixShaderOOB) {
	static void __fastcall callback(X360SurfaceShaderList * _this, uintptr_t edx, int param_2, int param_3) {
		int* piVar1;
		int iVar2;
		int iVar3;
		int iVar4;
		int* piVar5;
		int local_4;

		auto DAT_007161c4 = *reinterpret_cast<uintptr_t*>(0x007161c4);

		iVar2 = **(int**)(*(int*)(DAT_007161c4 + 0x34) + 4 + param_2 * 0xc);
		if ((iVar2 != 0) && (local_4 = 0, 0 < *(int*)(iVar2 + 0x18))) {
			piVar5 = (int*)(iVar2 + 0x3c);
			do {
				iVar3 = _this->unk_buffer[*piVar5];
				if (iVar3 != 0) {
					// One here...
					if (param_3 < 4) {
						piVar1 = (int*)(iVar3 + 0x20 + param_3 * 0xc);
						iVar4 = _this->surface_shaders[param_3].next_idx;
						if (*piVar1 != iVar4) {
							*piVar1 = iVar4;
							iVar3 = *(int*)(iVar3 + 8);
							iVar4 = *(int*)(iVar3 + 0x3c);
							// One here...
							if (-1 < iVar4 && iVar4 < 4) {
								callback(_this, 8, param_3, iVar4);
							}
							iVar3 = *(int*)(iVar3 + 0x40);
							// And one here...
							if (-1 < iVar3 && iVar4 < 4) {
								callback(_this, 0, param_3, iVar3);
							}
						}
					}
				}
				local_4 = local_4 + 1;
				piVar5 = piVar5 + 1;
			} while (local_4 < *(int*)(iVar2 + 0x18));
		}
		return;
	}
};


/*
Ensures every buffer allocated by the game is zero-initialized. #ifdef'd out of release builds.
*/
DefineReplacementHook(ZeroInitializeMalloc) {
	static void* __cdecl callback(std::size_t len) {
		auto ptr = original(len);
		if (ptr != nullptr && len > 0) {
			memset(ptr, len, 0);
		}
		return ptr;
	}
};


void misc::install() {
#ifdef _DEBUG
	ZeroInitializeMalloc::install_at_ptr(0x0063f5f1);
#endif // _DEBUG
	FixShaderOOB::install_at_ptr(0x00565d40);
}
