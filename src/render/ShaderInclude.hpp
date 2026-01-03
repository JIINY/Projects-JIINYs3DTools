#pragma once
#include <d3dcompiler.h>
#include <string>
#include <string_view>
#include <filesystem>


class ShaderInclude : public ID3DInclude 
{
public:
    ShaderInclude(std::string_view shaderPath);

    HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override;
    HRESULT __stdcall Close(LPCVOID pData) override;

    static std::string loadSourceCode(const std::filesystem::path& path);

private:
    std::filesystem::path shaderDir_;
};
