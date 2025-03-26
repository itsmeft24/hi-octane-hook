#include "misc.hpp"

#include "core/game/block_allocator.hpp"
#include "core/game/cars_game.hpp"
#include "core/game/serializable_interface.hpp"
#include "core/logging.hpp"
#include "sunset/sunset.hpp"

struct SurfaceShader {
	int next_idx;
	BlockAllocator* block_allocator;
};

struct X360SurfaceShaderList {
	void* vtable;
	int surface_shader_pass_count;
	struct X360SurfaceShaderPass* surface_shader_passes[0x1000];
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
				iVar3 = reinterpret_cast<int>(_this->surface_shader_passes[*piVar5]);
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

/*
Increases the size of the scratch buffer used to determine the required length of the serialized CarsRecordLibrary.
*/
DefineReplacementHook(FixCarsRecordLibrary) {
	static int __fastcall callback(SerializableInterface* _this) {
		std::vector<std::uint8_t> buffer(0x10000);
		int size = _this->Serialize(buffer.data(), buffer.size());
		return size;
	}
};

/*
Ensures that the streaming_package_index is valid, preventing crashes when jumping to join points.
(Only occurs when the scene is streamed.)
*/
DefineReplacementHook(StreamingManager_BlockUntilAllSectionsAreReady) {
	static void __fastcall callback(struct StreamingManager* _this, uintptr_t edx, int streaming_package_index) {
		if (streaming_package_index != -1) {
			original(_this, edx, streaming_package_index);
		}
	}
};

void misc::install() {
#ifdef _DEBUG
	ZeroInitializeMalloc::install_at_ptr(0x0063f5f1);
#endif // _DEBUG
	FixShaderOOB::install_at_ptr(0x00565d40);
	// Prevents crashes when jumping to a join point.
	StreamingManager_BlockUntilAllSectionsAreReady::install_at_ptr(0x005b66e0);
	
	FixCarsRecordLibrary::install_at_ptr(0x0048e580);

	logging::log("[misc::install] Successfully installed patch!");
}
