#pragma once
#include <cstdint>
#include "core/game/parameter_block.hpp"
#include "core/game/gfx/x360_shader.hpp"
#include "core/game/container_linked_list.hpp"

namespace HDRPatch {

#pragma pack(push, 1)
	enum PixelShaderConstant {
		PS_HDR_Threshold = 16,
		PS_HDR_Speed = 17,
		PS_HDR_Mix = 18,
		PS_HDR_UVOffset = 19,
		PS_HDR_OnePixelOffset = 20,
		PS_HDR_MaxTexCoord = 21,
		PS_HDR_BlurKernel = 22,
		PS_HDR_BlurOffset = 37,
	};

	// the struct layout of the base class
	struct X360FilterAlgorithm {
		struct VTable {
			X360FilterAlgorithm*(__thiscall* Deleter)(X360FilterAlgorithm*, unsigned char);
			unsigned int(__thiscall* SendData)(X360FilterAlgorithm*);
			unsigned int(__thiscall* ReadFromFile)(X360FilterAlgorithm*, uintptr_t);
			void(__thiscall* Initialize)(X360FilterAlgorithm*, char*, ParameterBlock*);
			void(__thiscall* Enable)(X360FilterAlgorithm*, unsigned int);
			void(__thiscall* Unk)(X360FilterAlgorithm*);
			void(__thiscall* UnkGetD3DDeviceIdk)(X360FilterAlgorithm*);
		} *vtbl;
		int unused;
		int render_target_width;
		int render_target_height;
		int render_target_format;
		char name[64];
		LinkedListNode<X360FilterAlgorithm*>* node;
		int is_enabled;
		int unk_geometry_entries_len;
		void* unk_geometry_entries;
	};
	static_assert(sizeof(X360FilterAlgorithm) == 100);
#pragma pack(pop)

	struct X360FilterHighDynamicRange {
		X360FilterAlgorithm base;
		int RotateBlur;
		int NumBlurPasses;
		float Threshold;
		float PositiveRate;
		float NegativeRate;
		float FinalMixHDR;
		float FinalMixLDR;
		float Scale;

		X360VertexShader* BlurVert;
		X360PixelShader* BlurPixel;

		X360VertexShader* BufferVert;
		X360PixelShader* BufferPixel;

		X360VertexShader* DownsampleVert;
		X360PixelShader* DownsamplePixel;

		X360VertexShader* FinalVert;
		X360PixelShader* FinalPixel;

		X360VertexShader* ThresholdVert;
		X360PixelShader* ThresholdPixel;
	public:
		static X360FilterHighDynamicRange* __fastcall Constructor(X360FilterHighDynamicRange* _this);
		static X360FilterHighDynamicRange* __fastcall Deleter(X360FilterHighDynamicRange* _this, uintptr_t edx, unsigned char);
		static unsigned int __fastcall SendData(X360FilterHighDynamicRange* _this);
		static void __fastcall Initialize(X360FilterHighDynamicRange* _this, uintptr_t edx, char*, ParameterBlock* pBlock);
	};
	void install();
};