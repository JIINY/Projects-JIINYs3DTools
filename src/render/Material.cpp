#include "Material.hpp"
#include <iostream>
#include <cassert>
using namespace std;


namespace Render 
{
    //static 멤버의 메모리 확보
    std::shared_ptr<VertexShader> Material::fallbackVS_ = nullptr;
    std::shared_ptr<PixelShader> Material::errorPS_Pink_ = nullptr;
    std::shared_ptr<PixelShader> Material::errorVS_Green_ = nullptr;
    std::shared_ptr<PixelShader> Material::wireframe_ = nullptr;
    std::shared_ptr<PixelShader> Material::default_ = nullptr;

    Material::Material() {}
    Material::~Material() {}

    bool Material::prepareDefaultShaders(ID3D11Device* device) 
    {
        if (fallbackVS_) { return false; }

        fallbackVS_ = std::make_shared<VertexShader>();
        bool isFallbackVS_Loaded = fallbackVS_->initialize(device, L"shaders/Unlit_Common/Fallback_VS.hlsl", "vsMain");
        assert(isFallbackVS_Loaded && "[에러] Fallback VS 셰이더 로드 실패");
        if (!isFallbackVS_Loaded)
        {
            MessageBoxA(nullptr, "CRITICAL: Fallback VS Load Failed!", "Engine Error", MB_OK);
            return false;
        }

        errorVS_Green_ = make_shared<PixelShader>();
        bool isErrorVS_Green_Loaded = errorVS_Green_->initialize(device, L"shaders/Unlit_Common/Error_PS_Green.hlsl", "psMain"); //VS에러, 적용 셰이더는 PS
        assert(isErrorVS_Green_Loaded && "[에러] Vertex에러 셰이더 로드 실패");
        if (!isErrorVS_Green_Loaded)
        {
            MessageBoxA(nullptr, "CRITICAL: Error VS Green Load Failed!", "Engine Error", MB_OK);
            return false;
        }

        errorPS_Pink_ = make_shared<PixelShader>();
        bool isErrorVS_Pink_Loaded = errorPS_Pink_->initialize(device, L"shaders/Unlit_Common/Error_PS_Pink.hlsl", "psMain");
        assert(isErrorVS_Pink_Loaded && "[에러] Pixel에러 셰이더 로드 실패");
        if (!isErrorVS_Pink_Loaded)
        {
            MessageBoxA(nullptr, "CRITICAL: Error VS Pink Load Failed!", "Engine Error", MB_OK);
            return false;
        }

        wireframe_ = make_shared<PixelShader>();
        bool isWireframe_Loaded = wireframe_->initialize(device, L"shaders/Unlit_Common/Wireframe_PS_DarkGray.hlsl", "psMain");
        assert(isWireframe_Loaded && "[에러] wire셰이더 로드 실패");
        if (!isWireframe_Loaded) 
        {
            MessageBoxA(nullptr, "CRITICAL: wireframe PS Load Failed!", "Engine Error", MB_OK);
            return false;
        }

        default_ = make_shared<PixelShader>();
        bool isDefault_Loaded = default_->initialize(device, L"shaders/Unlit_Common/Default_PS_LightGray.hlsl", "psMain");
        assert(isDefault_Loaded && "[에러] 기본PS 셰이더 로드 실패");
        if (!isDefault_Loaded) 
        {
            MessageBoxA(nullptr, "CRITICAL: Default PS Load Failed!", "Engine Error", MB_OK);
            return false;
        }

        return true;
    }

    void Material::initialize(ID3D11Device* device) 
    {
        vertexShader_ = fallbackVS_;
        pixelShader_ = errorPS_Pink_;
    }

    void Material::loadVertexShader(ID3D11Device* device, wstring_view filepath, string_view entryPoint) 
    {
        auto newVS = std::make_shared<VertexShader>();
        if (newVS->initialize(device, filepath, entryPoint)) 
        {
            vertexShader_ = newVS;
        }
        else 
        {
            cout << "[ERROR] VS Load Failed. using Green VS ErrorColor." << endl;
            vertexShader_ = fallbackVS_;
            pixelShader_ = errorVS_Green_;
        }
    }

    void Material::loadPixelShader(ID3D11Device* device, wstring_view filepath, string_view entryPoint) 
    {
        auto newPS = std::make_shared<PixelShader>();
        if (newPS->initialize(device, filepath, entryPoint))
        {
            pixelShader_ = newPS;
        }
        else
        {
            cout << "[ERROR] PS Load Failed. using Pink PS ErrorColor." << endl;
            pixelShader_ = errorPS_Pink_;
        }
    }

    void Material::bind(ID3D11DeviceContext* context) 
    {
        if (vertexShader_) { vertexShader_->bind(context); }
        if (pixelShader_) { pixelShader_->bind(context); }
    }
}
