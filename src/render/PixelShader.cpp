#include "PixelShader.hpp"
#include <d3dcompiler.h>
#include <cassert>
#include <string>
#include "ShaderInclude.hpp"
#pragma comment(lib, "d3dcompiler.lib")
using namespace std;


namespace Render 
{
    bool PixelShader::initialize(ID3D11Device* device, wstring_view filepath, string_view entryPoint) 
    {
        //1. PS컴파일
        if (!compile(filepath, entryPoint, shaderBlob_.GetAddressOf()))
        {
            return false;
        }

        //2. PixelShader 객체 생성
        HRESULT hr = device->CreatePixelShader(shaderBlob_->GetBufferPointer(), shaderBlob_->GetBufferSize(), nullptr, shader_.GetAddressOf());
        if (FAILED(hr)) 
        {
            return false;
        }

        return true;
    }

    void PixelShader::bind(ID3D11DeviceContext* context) 
    {
        if (context) 
        {
            context->PSSetShader(shader_.Get(), nullptr, 0);
        }
    }

    void PixelShader::unbind(ID3D11DeviceContext* context) 
    {
        if (context) 
        {
            context->PSSetShader(nullptr, nullptr, 0);
        }
    }

    void PixelShader::setConstantBuffer(uint32_t slot, ShaderStage stage, shared_ptr<ConstantBuffer> buffer, ID3D11DeviceContext* context)
    {
        if (context && buffer)
        { 
            ID3D11Buffer* cb = buffer->getBuffer();
            context->PSSetConstantBuffers(slot, 1, &cb);
        }
    }

    bool PixelShader::compile(std::wstring_view filepath, std::string_view entryPoint, ID3DBlob** blob)
    {
        DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#if defined(_DEBUG)
        flags |= D3DCOMPILE_DEBUG;
        flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

        wstring wsPath(filepath);
        string strPath(wsPath.begin(), wsPath.end());
        string sourceCode = ShaderInclude::loadSourceCode(strPath);

        //공통 include파일 처리
        ShaderInclude includeHandler(strPath);

        //D3DCompile 호출: BOM처리하기 때문에 FromFile 아님
        HRESULT hr = D3DCompile(
            sourceCode.c_str(),
            sourceCode.size(),
            strPath.c_str(),
            nullptr,
            &includeHandler,
            entryPoint.data(),
            "ps_5_0",
            flags,
            0,
            blob,
            errorBlob.GetAddressOf()
        );

        if (FAILED(hr))
        {
            if (errorBlob)
            {
                string errorMsg((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize());
                OutputDebugStringA("Shader Comfile Error=====================================\n");
                OutputDebugStringA(errorMsg.c_str());
                OutputDebugStringA("=========================================================\n");
                assert(0 && "셰이더 컴파일 실패, [출력]창을 확인하세요");
            }
            else            
            {
                OutputDebugStringA("에러메세지가 없습니다. include 경로 확인 필요");
                assert(0 && "셰이더 컴파일 실패, include 경로를 확인하세요");
            }
            return false;
        }

        return true;
    }
}
