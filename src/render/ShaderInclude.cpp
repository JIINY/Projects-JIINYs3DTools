#include "ShaderInclude.hpp"
#include <cassert>
#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <Windows.h>
using namespace std;


ShaderInclude::ShaderInclude(string_view shaderPath) 
{
    shaderDir_ = filesystem::path(shaderPath).parent_path();
}

HRESULT __stdcall ShaderInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
{
    filesystem::path finalPath = shaderDir_ / pFileName;

    if (!filesystem::exists(finalPath)) 
    {
        filesystem::path commonPath = shaderDir_.parent_path() / "Common" / pFileName;

        if (filesystem::exists(commonPath)) 
        {
            finalPath = commonPath;
        }
    }

    string source = loadSourceCode(finalPath);
    if (source.empty()) 
    {
        wstring wPath = finalPath.wstring();
        wstring errorMsg = L"[에러] 경로: " + wPath + L"\n";
        OutputDebugStringW(errorMsg.c_str());
        assert(0 && "[에러] Include 파일을 찾을 수 없음, 출력창의 경로를 확인하세요");

        return E_FAIL;
    }

    char* buffer = new char[source.size()];
    memcpy(buffer, source.data(), source.size());

    *ppData = buffer;
    *pBytes = static_cast<UINT>(source.size());

    return S_OK;
}

HRESULT __stdcall ShaderInclude::Close(LPCVOID pData) 
{
    char* pBuffer = (char*)pData;
    delete[] pBuffer;
    return S_OK;
}

string ShaderInclude::loadSourceCode(const filesystem::path& path) 
{
    if (!filesystem::exists(path)) 
    {
        string errorMsg = "[에러] 경로: " + path.string() + "\n";
        OutputDebugStringA(errorMsg.c_str());
        assert(0 && "[에러] 셰이더 파일이 없습니다. 출력창을 확인하세요");
        return "";
    }

    ifstream file(path, ios::binary | ios::ate);
    if (!file.is_open()) 
    {
        assert(0 && "[에러] 셰이더 파일 열기 실패");
        return "";
    }

    //파일 크기 확인 및 버퍼 생성
    size_t fileSize = file.tellg();
    vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    const char* sourceCode = buffer.data();
    size_t sourceSize = fileSize;

    //앞 3바이트가 UTF-8 BOM인지 확인
    if (fileSize >= 3 &&
        (unsigned char)buffer[0] == 0xEF &&
        (unsigned char)buffer[1] == 0xBB &&
        (unsigned char)buffer[2] == 0xBF)
    {
        sourceCode += 3;
        sourceSize -= 3;
    }

    return string(sourceCode, sourceSize);
}
