
#include <iostream>
#include <d3d9.h>
#include <directxmath.h>
#include "../Globals.h"
#include "../ConfigManager.h"

#include "HDR.h"
#include "../Logging.h"
#include "../HookFunction.h"

namespace HDRPatch {

    DeclareFunction(void*, __cdecl, operator_new, 0x0063eb87, size_t);
    DeclareFunction(void, __cdecl, operator_delete, 0x0063f514, void*);
    // the base class's constructor.
    DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterAlgorithm_Constructor, 0x0040ce10, X360FilterAlgorithm*);
    DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterAlgorithm_Destructor, 0x0040d190, X360FilterAlgorithm*);
	DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterAlgorithm_Initialize, 0x0040ce50, X360FilterAlgorithm*, char*, uintptr_t);
	DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterAlgorithm_CreateGeometry, 0x0040cec0, X360FilterAlgorithm*, unsigned int);
	DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterAlgorithm_InitializeGeometry, 0x0040cf70, X360FilterAlgorithm*, unsigned int, uintptr_t);
	DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterAlgorithm_RenderGeometry, 0x0040d230, X360FilterAlgorithm*, unsigned int);

	// all the other derived constructors cuz we reimplement X360FilterStream::ReadFromFile
    DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterNull_Constructor,0x0040edc0, void*);
    DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterFrameBufferToTexture_Constructor, 0x0040d270, void*);
    DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterMotionBlur_Constructor, 0x0040dfd0, void*);
    DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterNightVision_Constructor, 0x0040e5e0, void*);
    DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterGhostImage_Constructor, 0x0040d830, void*);
    DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterSunflare_Constructor, 0x0040ee40, void*);
    DeclareFunction(X360FilterAlgorithm*, __thiscall, X360FilterNoise_Constructor, 0x0040ea70, void*);
    DeclareFunction(unsigned int, __thiscall, X360VideoCard_IsFXEnabled, 0x00415440, void*);
    DeclareFunction(unsigned int, __thiscall, ParameterBlock_GetParameterDefaultStr, 0x005881a0, uintptr_t, const char*, const char*, char*, int);
    DeclareFunction(unsigned int, __thiscall, ParameterBlock_GetParameterDefaultInt, 0x00588250, uintptr_t, const char*, int, int*);
    DeclareFunction(unsigned int, __thiscall, ParameterBlock_GetParameterDefaultFloat, 0x005882b0, uintptr_t, const char*, float, float*);
    DeclareFunction(void, __thiscall, X360FilterStream_ReadFromFile, 0x0040b5b0, void*, uintptr_t, uintptr_t);
    DeclareFunction(bool, __thiscall, Unk_GetVertexShader, 0x00410fa0, void*, char*, int);
    DeclareFunction(bool, __thiscall, Unk_GetPixelShader, 0x004114b0, void*, char*, int);
    DeclareFunction(void, __thiscall, Unk_SetVertexShader, 0x004110a0, void*, void*, int);
    DeclareFunction(void, __thiscall, Unk_SetPixelShader, 0x00411530, void*, void*);

	DeclareFunction(uintptr_t, __thiscall, PrepareTextureSet, 0x0040de30, uintptr_t, int);

	DeclareFunction(void, __thiscall, SetTextureSlot, 0x0040f5b0, uintptr_t, int);
	DeclareFunction(void, __thiscall, SomethingIdk, 0x0040de00, uintptr_t, uintptr_t, int);
	
	// for grabbing the game's IDirect3DDevice9.
	inline IDirect3DDevice9* d3d9() {
		return *reinterpret_cast<IDirect3DDevice9**>(0x006fd3f0);
	}

	// for grabbing the game's X360RenderTarget
	inline uintptr_t RenderTarget() {
		return *reinterpret_cast<uintptr_t*>(0x00716268);
	}

	inline void* InitVertexShader(const std::string& shader_label) {
		void* memory = operator_new(0x10C);
		*reinterpret_cast<unsigned long long*>(memory) = 0;
		Unk_GetVertexShader(memory, const_cast<char*>(shader_label.c_str()), 0);
		return memory;
	}

	inline void* InitPixelShader(const std::string& shader_label) {
		void* memory = operator_new(0x4);
		*reinterpret_cast<unsigned int*>(memory) = 0;
		Unk_GetPixelShader(memory, const_cast<char*>(shader_label.c_str()), 0);
		return memory;
	}

	inline void SetShaders(void* vertex, void* pixel) {
		uintptr_t g_RenderTarget = RenderTarget();
		Unk_SetVertexShader(*reinterpret_cast<void**>(g_RenderTarget + 0xC4), vertex, 0);
		Unk_SetPixelShader(*reinterpret_cast<void**>(g_RenderTarget + 200), pixel);
	}

	// a reimplementation of X360FilterStream::ReadFromFile that adds a case for our HDR filter pass.
	void __fastcall X360FilterStream_ReadFromFileHook(void* filter_stream, uintptr_t edx, uintptr_t pBlock, uintptr_t param_2)
	{
		X360FilterAlgorithm* filter_algorithm = nullptr;
		void* memory = nullptr;
		char algorithm_label[0x40]{};

		uintptr_t test = *reinterpret_cast<uintptr_t*>(pBlock + 0x3C);

		ParameterBlock_GetParameterDefaultStr(pBlock, "FilterAlgorithm", "", algorithm_label, 0x40);
		char* this_algorithm = reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(filter_stream) + 0xC);
		strcpy(this_algorithm, algorithm_label);
		if (stricmp(algorithm_label, "Null") == 0 || !X360VideoCard_IsFXEnabled(*reinterpret_cast<void**>(0x006ff394))) {
			strcpy(this_algorithm, "Null");
			memory = operator_new(100);
			if (memory != nullptr) {
				filter_algorithm = X360FilterNull_Constructor(memory);
			}
			else {
				filter_algorithm = nullptr;
			}
		}
		else if (stricmp(algorithm_label, "FrameBufferToTexture") == 0) {
			memory = operator_new(0x68);
			if (memory != nullptr) {
				filter_algorithm = X360FilterFrameBufferToTexture_Constructor(memory);
			}
			else {
				filter_algorithm = nullptr;
			}
		}
		else if (stricmp(algorithm_label, "MotionBlur") == 0) {
			memory = operator_new(0x84);
			if (memory != nullptr) {
				filter_algorithm = X360FilterMotionBlur_Constructor(memory);
			}
			else {
				filter_algorithm = nullptr;
			}
		}
		else if (stricmp(algorithm_label, "HighDynamicRange") == 0) {
			memory = operator_new(sizeof(X360FilterHighDynamicRange));
			if (memory != nullptr) {
				filter_algorithm = reinterpret_cast<X360FilterAlgorithm*>(X360FilterHighDynamicRange::Constructor(reinterpret_cast<X360FilterHighDynamicRange*>(memory)));
			}
			else {
				filter_algorithm = nullptr;
			}
		}
		else if (stricmp(algorithm_label, "NightVision") == 0) {
			memory = operator_new(0xc0);
			if (memory != nullptr) {
				filter_algorithm = X360FilterNightVision_Constructor(memory);
			}
			else {
				filter_algorithm = nullptr;
			}
		}
		else if (stricmp(algorithm_label, "GhostImage") == 0) {
			memory = operator_new(0x80);
			if (memory != nullptr) {
				filter_algorithm = X360FilterGhostImage_Constructor(memory);
			}
			else {
				filter_algorithm = nullptr;
			}
		}
		else if (stricmp(algorithm_label, "Sunflare") == 0) {
			memory = operator_new(0x78);
			if (memory != nullptr) {
				filter_algorithm = X360FilterSunflare_Constructor(memory);
			}
			else {
				filter_algorithm = nullptr;
			}
		}
		else if (stricmp(algorithm_label, "Noise") == 0) {
			memory = operator_new(0x84);
			if (memory != nullptr) {
				filter_algorithm = X360FilterNoise_Constructor(memory);
			}
			else {
				filter_algorithm = nullptr;
			}
		}
		else {
			strcpy(this_algorithm, "Null");
			memory = operator_new(100);
			if (memory != nullptr) {
				filter_algorithm = X360FilterNull_Constructor(memory);
			}
			else {
				filter_algorithm = nullptr;
			}
		}
		if (filter_algorithm != nullptr) {
			auto init = filter_algorithm->vtbl->Initialize;
			auto init_fastcall = reinterpret_cast<void(__fastcall*)(X360FilterAlgorithm*, uintptr_t, char*, uintptr_t)>(init);
			init_fastcall(filter_algorithm, 0, this_algorithm, pBlock);
			*reinterpret_cast<X360FilterAlgorithm**>(reinterpret_cast<uintptr_t>(filter_stream) + 0x4) = filter_algorithm;
		}
		return;
	}

	X360FilterAlgorithm::VTable HDR_VTable = {
		 reinterpret_cast<decltype(X360FilterAlgorithm::VTable::Deleter)>(X360FilterHighDynamicRange::Deleter),
		 reinterpret_cast<decltype(X360FilterAlgorithm::VTable::SendData)>(X360FilterHighDynamicRange::SendData),
		 reinterpret_cast<decltype(X360FilterAlgorithm::VTable::ReadFromFile)>(0x00595e30),
		 reinterpret_cast<decltype(X360FilterAlgorithm::VTable::Initialize)>(X360FilterHighDynamicRange::Initialize),
		 reinterpret_cast<decltype(X360FilterAlgorithm::VTable::Enable)>(0x0040ceb0)
	};

	X360FilterHighDynamicRange* __fastcall X360FilterHighDynamicRange::Constructor(X360FilterHighDynamicRange* _this) {
		X360FilterAlgorithm_Constructor(reinterpret_cast<X360FilterAlgorithm*>(_this));
		_this->base.vtbl = &HDR_VTable;
		return _this;
	}

	X360FilterHighDynamicRange* __fastcall X360FilterHighDynamicRange::Deleter(X360FilterHighDynamicRange* _this, uintptr_t edx, unsigned long long param_2)
	{
        // reset the vtable
		_this->base.vtbl = reinterpret_cast<X360FilterAlgorithm::VTable*>(0x00675C40);
        // call the base destructor
        X360FilterAlgorithm_Destructor(&_this->base);
        if ((param_2 & 1) != 0) {
            operator_delete(_this);
        }
		return _this;
	}

	unsigned int __fastcall X360FilterHighDynamicRange::SendData(X360FilterHighDynamicRange* _this) {
		// Downsampling
		{
			// Idk what this is
			uintptr_t g_RenderTarget = RenderTarget();
			SomethingIdk(0x006fd2b8, g_RenderTarget + 0x98, 4);

			// Set shaders.
			SetShaders(_this->DownsampleVert, _this->DownsamplePixel);

			// Set shader constants.
			auto window_width = ConfigManager::IsWidescreenEnabled ? 640 : ConfigManager::DesiredWindowWidth;
			auto window_height = ConfigManager::IsWidescreenEnabled ? 480 : ConfigManager::DesiredWindowHeight;

			float OnePixelOffset[] = { 1.0 / window_width, 1.0 / window_height, 0, 0 };
			d3d9()->SetPixelShaderConstantF(PixelShaderConstant::PS_HDR_OnePixelOffset, OnePixelOffset, 1);

			float MaxTexCoord[] = { 1.0, 1.0, 0, 0 };
			d3d9()->SetPixelShaderConstantF(PixelShaderConstant::PS_HDR_MaxTexCoord, MaxTexCoord, 1);

			// Set texture slots and render.
			uintptr_t this_00 = PrepareTextureSet(0x006fd2b8, 4);
			for (int x = 0; x < 4; x++) {
				SetTextureSlot(this_00, x);
			}

			X360FilterAlgorithm_RenderGeometry(&_this->base, 0);
		}
		// Thresholding
		{
		}
		// Buffering
		{
		}
		// Blurring
		{
		}
		// Final compositing
		{
		}
		/*
		float Threshold[] = { _this->Threshold, 0, 0, 0 };
		lpDirect3D9Device->SetPixelShaderConstantF(PixelShaderConstant::PS_HDR_Threshold, Threshold, 1);
		float Speed[] = { _this->PositiveRate, _this->NegativeRate, 0, 0 };
		lpDirect3D9Device->SetPixelShaderConstantF(PixelShaderConstant::PS_HDR_Speed, Speed, 1);
		float FinalMix[] = { _this->FinalMixHDR, _this->FinalMixLDR, 0, 0 };
		lpDirect3D9Device->SetPixelShaderConstantF(PixelShaderConstant::PS_HDR_Mix, FinalMix, 1);

		

		
		lpDirect3D9Device->SetPixelShaderConstantF(PixelShaderConstant::PS_HDR_BlurKernel, nullptr, 15);
		FUN_0040de00(DAT_006fd2b8, g_RenderTarget + 0x98, 4);
		SetShaders(_this->DownsampleVert, _this->DownsamplePixel);
		this_00 = (void*)FUN_0040de30(DAT_006fd2b8, 4);

		X360FilterAlgorithm::RenderGeometry(0);
		*/
		return 1;
	}

	void __fastcall X360FilterHighDynamicRange::Initialize(X360FilterHighDynamicRange* _this, uintptr_t edx, char* name, uintptr_t parameterBlock)
	{
		X360FilterAlgorithm_Initialize(&_this->base, name, parameterBlock);
		ParameterBlock_GetParameterDefaultInt(parameterBlock, "RotateBlur", _this->RotateBlur, &_this->RotateBlur);
		ParameterBlock_GetParameterDefaultInt(parameterBlock, "NumBlurPasses", _this->NumBlurPasses, &_this->NumBlurPasses);
		ParameterBlock_GetParameterDefaultFloat(parameterBlock, "Threshold", _this->Threshold, &_this->Threshold);
		ParameterBlock_GetParameterDefaultFloat(parameterBlock, "PositiveRate", _this->PositiveRate, &_this->PositiveRate);
		ParameterBlock_GetParameterDefaultFloat(parameterBlock, "NegativeRate", _this->NegativeRate, &_this->NegativeRate);
		ParameterBlock_GetParameterDefaultFloat(parameterBlock, "FinalMixHDR", _this->FinalMixHDR, &_this->FinalMixHDR);
		ParameterBlock_GetParameterDefaultFloat(parameterBlock, "FinalMixLDR", _this->FinalMixLDR, &_this->FinalMixLDR);
		ParameterBlock_GetParameterDefaultFloat(parameterBlock, "Scale", _this->Scale, &_this->Scale);
		_this->NumBlurPasses = std::max(std::min(_this->NumBlurPasses, 8), 1);
		
		_this->BlurVert = InitVertexShader("360_Filter_HDR_Blur.hlsl");
		_this->BlurPixel = InitPixelShader("360_Filter_HDR_Blur.hlsl");
		
		_this->BufferVert = InitVertexShader("360_Filter_HDR_Buffer.hlsl");
		_this->BufferPixel = InitPixelShader("360_Filter_HDR_Buffer.hlsl");
		
		_this->DownsampleVert = InitVertexShader("360_Filter_HDR_Downsample.hlsl");
		_this->DownsamplePixel = InitPixelShader("360_Filter_HDR_Downsample.hlsl");
		
		_this->FinalVert = InitVertexShader("360_Filter_HDR_Final.hlsl");
		_this->FinalPixel = InitPixelShader("360_Filter_HDR_Final.hlsl");
		
		_this->ThresholdVert = InitVertexShader("360_Filter_HDR_Threshold.hlsl");
		_this->ThresholdPixel = InitPixelShader("360_Filter_HDR_Threshold.hlsl");
		
		X360FilterAlgorithm_CreateGeometry(&_this->base, 1);
		X360FilterAlgorithm_InitializeGeometry(&_this->base, 0, 0x00675d08);

		_this->base.isEnabled = 1;
	}

	void Install() {
		HookFunction((void*&)X360FilterStream_ReadFromFile, X360FilterStream_ReadFromFileHook, 7, FunctionHookType::EntireReplacement);
	}
};