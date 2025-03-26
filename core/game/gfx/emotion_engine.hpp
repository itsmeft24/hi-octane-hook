#pragma once
#include <d3d9.h>
#include <type_traits>

namespace EE {
	class RefObject {
	protected:
		int ref_count;
	public:
		virtual ~RefObject();
		/*
		inline void AddRef() {
			ref_count++;
		}
		inline void Release() {
			ref_count--;
			if (ref_count == 0) {
				delete this;
			}
		}
		*/
	};

	template <class T> class List {
	public:
		T* data;
		std::size_t capacity;
		std::size_t len;
	public:
		/*
		List(std::size_t initial_capacity) {
			capacity = initial_capacity;
			data = malloc(initial_capacity);
		}
		void InsertRaw(std::size_t index, T element) {
		}
		void InsertRefObject(std::size_t index, T element) {
		}
		inline ~List() {
			for (std::size_t i = 0; i < len; i++) {
				reinterpret_cast<RefObject*>(data[i])->Release();
			}
			free(data);
		}
		*/
	};

	class D3DDepthStencil : public RefObject {
	private:
		D3DFORMAT surface_format;
		bool fullscreen;
		bool windowed;
		unsigned int windowed_quality_level;
		unsigned int unk_bitfield_windowed;
		unsigned int fullscreen_quality_level;
		unsigned int unk_bitfield_fullscreen;
	public:
		// void ResolveQualityLevel(IDirect3D9* d3d9, D3DDEVTYPE dev_type, D3DFORMAT fmt, int windowed);
	};

	static_assert(sizeof(D3DDepthStencil) == 0x20);

	class D3DRenderTarget : public RefObject {
	private:
		D3DFORMAT format;
		List<D3DDepthStencil*> depth_stencils;
		int unused;
	};

	static_assert(sizeof(D3DRenderTarget) == 0x1C);

	class D3DBackBuffer : public RefObject {
	private:
		D3DFORMAT surface_format;
		bool fullscreen;
		bool windowed;
		List<D3DRenderTarget*> render_target_list;
		int unused;
	};

	static_assert(sizeof(D3DBackBuffer) == 0x20);

	class D3DDisplayMode : public RefObject {
	public:
		D3DFORMAT format;
		unsigned int width;
		unsigned int height;
		List<unsigned int> refresh_rate_list;
	};

	static_assert(sizeof(D3DDisplayMode) == 0x20);

	class D3DAdapter : public RefObject {
	public:
		unsigned int adapter_index;
		D3DADAPTER_IDENTIFIER9 identifier;
		D3DDISPLAYMODE display_mode;
		D3DCAPS9 capabilities;
		List<D3DDisplayMode*> display_mode_list;
		List<D3DBackBuffer*> back_buffer_list;
		int unused;
	};

	static_assert(sizeof(D3DAdapter) == 0x5b4);

	class D3DDeviceManager : public RefObject {
	public:
		IDirect3D9* d3d9;
		List<D3DAdapter*> adapter_list;
		unsigned int adapter_index;
		D3DPRESENT_PARAMETERS presentation_params;
		D3DFORMAT surface_format;
		D3DDEVTYPE device_type;
		unsigned int behavior_flag_2;
		D3DMULTISAMPLE_TYPE multi_sample_type;
		IDirect3DDevice9* device;
		IDirect3DSurface9* back_buffer;
		IDirect3DSurface9* depth_stencil_surface;
		bool windowed;
		int n_depth_stencil_bits;
		int n_depth_bits;
		int n_stencil_bits;
		int back_buffer_count;
		int back_buffer_width;
		int back_buffer_height;
		unsigned int min_vertex_shader_version;
		unsigned int min_pixel_shader_version;
		unsigned int behavior_flag_1;
		bool is_reference_device;
	public:
		/*
		D3DDeviceManager(IDirect3D9* _d3d9);
		D3DDeviceManager(const D3DDeviceManager&) = delete;
		D3DDeviceManager& operator=(const D3DDeviceManager&) = delete;
		virtual ~D3DDeviceManager() override;

		void SetMultiSampleType(bool enable);
		int SetPresentationParams(HWND hwnd, D3DAdapter* adapter);
		void SetBackBufferResolution(int w, int h);
		int SetBehaviorFlag(HWND hwnd, int adapter_index);
		void ResolveFrameBuffer(IDirect3DTexture9* texture);
		void ResetBackBufferAndDepthStencilSurface();
		void Reset();
		void ReleaseResources();
		int IsResolutionAvailable(int w, int h);
		int IsMultiSamplingEnabled();
		void Initialize();
		char* GetRelativeShaderPath();
		IDirect3DTexture9* GetBackBuffer();
		int Unk(D3DAdapter* adapter);
		int Unk2(D3DAdapter* adapter, int back_buffer_index);
		int CreateAdapters(HWND hwnd);
		int Create();
		void CheckAdapterDisplayMode(D3DAdapter* adapter, D3DFORMAT fmt, int* w, int* h);
		*/
	};

	static_assert(sizeof(D3DDeviceManager) == 0x9c);
};

inline EE::D3DDeviceManager** lpD3DDeviceManager = reinterpret_cast<EE::D3DDeviceManager**>(0x006fd3f8);
