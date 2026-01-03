#pragma once
#include "IShader.hpp"
#include <memory>
#include <string>
#include <d3d11.h>
#include <wrl/client.h>
#include "ConstantBuffer.hpp"


namespace Render 
{
    class PixelShader : public IShader
    {
    public:
        PixelShader();
        virtual ~PixelShader();

        bool initialize(ID3D11Device* device, std::wstring_view filepath, std::string_view entryPoint = "psMain");

        void bind(ID3D11DeviceContext* context) override;
        void unbind(ID3D11DeviceContext* context) override;

        void setConstantBuffer(uint32_t slot, ShaderStage stage, std::shared_ptr<ConstantBuffer> buffer, ID3D11DeviceContext* context);
        ID3DBlob* getBlob() const { return shaderBlob_.Get(); } //바이너리 데이터 접근

    private:
        Microsoft::WRL::ComPtr<ID3D11PixelShader> shader_;
        Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob_;

        bool compile(std::wstring_view filepath, std::string_view entryPoint, ID3DBlob** blob);
    };
}
