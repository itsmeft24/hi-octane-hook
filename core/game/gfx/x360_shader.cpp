#include "core/globals.hpp"
#include "x360_shader.hpp"
#include <cstddef>

DeclareFunction(IDirect3DPixelShader9*, __thiscall, X360ShaderManager_GetPS, 0x004159c0, void*, const char*);
DeclareFunction(IDirect3DVertexShader9*, __thiscall, X360ShaderManager_GetVS, 0x00415990, void*, const char*);

inline auto lpX360ShaderManager = reinterpret_cast<void**>(0x006fd3fc);

X360PixelShader::X360PixelShader()
{
    inner = nullptr;
}

X360PixelShader::~X360PixelShader()
{
    inner = nullptr;
}

bool X360PixelShader::Create(const char* name, int unused)
{
    if (this->inner != nullptr) {
        this->inner->Release();
        this->inner = nullptr;
    }

    this->inner = X360ShaderManager_GetPS(*lpX360ShaderManager, name);
    if (this->inner != nullptr) {
        this->inner->AddRef();
    }
    return this->inner != nullptr;
}

X360VertexShader::X360VertexShader()
{
    inner[0] = nullptr;
    inner[1] = nullptr;
}

X360VertexShader::~X360VertexShader()
{
    inner[0] = nullptr;
    inner[1] = nullptr;
}

bool X360VertexShader::Create(const char* name, int index)
{
    strcpy_s(this->name, name);
    for (std::size_t i = 0; i < 260; i++) {
        if (this->name[i] == 0) break;
        this->name[i] = tolower(this->name[i]);
    }

    if (this->inner[index] != nullptr) {
        this->inner[index]->Release();
    }

    this->inner[index] = X360ShaderManager_GetVS(*lpX360ShaderManager, name);
    if (this->inner[index] != nullptr) {
        this->inner[index]->AddRef();
    }
    return this->inner[index] != nullptr;
}