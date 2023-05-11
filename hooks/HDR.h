#pragma once
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
			X360FilterAlgorithm*(*__thiscall Deleter)(X360FilterAlgorithm*, unsigned long long);
			unsigned int(*__thiscall SendData)(X360FilterAlgorithm*);
			unsigned int(*__thiscall ReadFromFile)(X360FilterAlgorithm*, uintptr_t);
			void(*__thiscall Initialize)(X360FilterAlgorithm*, char*, uintptr_t);
			void(*__thiscall Enable)(X360FilterAlgorithm*, unsigned int);
		} *vtbl;
		char padding[0x54];
		unsigned int isEnabled;
		char padding2[0x8];
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

		void* BlurVert;
		void* BlurPixel;

		void* BufferVert;
		void* BufferPixel;

		void* DownsampleVert;
		void* DownsamplePixel;

		void* FinalVert;
		void* FinalPixel;

		void* ThresholdVert;
		void* ThresholdPixel;
	public:
		static X360FilterHighDynamicRange* __fastcall Constructor(X360FilterHighDynamicRange* _this);
		static X360FilterHighDynamicRange* __fastcall Deleter(X360FilterHighDynamicRange* _this, uintptr_t edx, unsigned long long);
		static unsigned int __fastcall SendData(X360FilterHighDynamicRange* _this);
		static void __fastcall Initialize(X360FilterHighDynamicRange* _this, uintptr_t edx, char*, uintptr_t parameterBlock);
	};
	void install();
};