#include "VertexShader.hpp"
#include <d3dcompiler.h>
#include <cassert>
#include <string>
#include "ConstantBuffer.hpp"
#include "ShaderInclude.hpp"
#pragma comment(lib, "d3dcompiler.lib")
using namespace std;

namespace Render 
{
    VertexShader::VertexShader() {}
    VertexShader::~VertexShader() {}

    bool VertexShader::initialize(ID3D11Device* device, wstring_view filepath, string_view entryPoint)
    {
        //1. VS컴파일
        if (!compile(filepath, entryPoint, shaderBlob_.GetAddressOf()))
        {
            return false;
        }

        //2. VertexShader 객체 생성
        HRESULT hr = device->CreateVertexShader(shaderBlob_->GetBufferPointer(), shaderBlob_->GetBufferSize(), nullptr, shader_.GetAddressOf());
        if (FAILED(hr))
        {
            return false;
        }

        //3. Input Layout생성
        if (!createInputLayout(device, shaderBlob_.Get()))
        {
            return false;
        }

        return true;
    }

    void VertexShader::bind(ID3D11DeviceContext* context) 
    {
        if (context)
        {
            context->IASetInputLayout(inputLayout_.Get());
            context->VSSetShader(shader_.Get(), nullptr, 0);
        }
    }

    void VertexShader::unbind(ID3D11DeviceContext* context)
    {
        if (context)
        {
            context->IASetInputLayout(nullptr);
            context->VSSetShader(nullptr, nullptr, 0);
        }
    }

    void VertexShader::setConstantBuffer(uint32_t slot, ShaderStage stage, shared_ptr<ConstantBuffer> buffer, ID3D11DeviceContext* context)
    {
        if (context && buffer)
        { 
            ID3D11Buffer* cb = buffer->getBuffer();
            context->VSSetConstantBuffers(slot, 1, &cb);
        }
    }

    bool VertexShader::compile(std::wstring_view filepath, std::string_view entryPoint, ID3DBlob** blob) 
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
            "vs_5_0",
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
                assert(false && "[에러] 셰이더 컴파일 실패, [출력]창을 확인하세요");
            }
            return false;
        }

        return true;
    }

    bool VertexShader::createInputLayout(ID3D11Device* device, ID3DBlob* vsBlob) 
    {
        D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };

        UINT numElements = ARRAYSIZE(layoutDesc);
        HRESULT hr = device->CreateInputLayout(layoutDesc, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), inputLayout_.GetAddressOf());

        return SUCCEEDED(hr);
    }
}
