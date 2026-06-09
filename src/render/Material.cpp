#include "Material.hpp"
#include <iostream>
#include <cassert>
#include "ui/UIInclude.hpp"
using namespace std;


namespace Render 
{
    //static 멤버의 메모리 확보
    std::shared_ptr<VertexShader> Material::fallbackVS_ = nullptr;
    std::shared_ptr<PixelShader> Material::errorPS_Pink_ = nullptr;
    std::shared_ptr<PixelShader> Material::errorVS_Green_ = nullptr;
    std::shared_ptr<PixelShader> Material::wireBlack_ = nullptr;
    std::shared_ptr<PixelShader> Material::wireRed_ = nullptr;
    std::shared_ptr<PixelShader> Material::default_ = nullptr;

    bool Material::prepareDefaultShaders(ID3D11Device* device) 
    {
        if (fallbackVS_) { return false; }

        fallbackVS_ = std::make_shared<VertexShader>();
        bool isFallbackVS_Loaded = fallbackVS_->initialize(device, L"resources/Unlit_Common/Fallback_VS.hlsl", "vsMain");
        assert(isFallbackVS_Loaded && "Fallback VS 셰이더 로드 실패");
        if (!isFallbackVS_Loaded)
        {
            MessageBoxA(nullptr, "CRITICAL: Fallback VS Load Failed!", "Engine Error", MB_OK);
            return false;
        }

        errorVS_Green_ = make_shared<PixelShader>();
        bool isErrorVS_Green_Loaded = errorVS_Green_->initialize(device, L"resources/Unlit_Common/Error_PS_Green.hlsl", "psMain"); //VS에러, 적용 셰이더는 PS
        assert(isErrorVS_Green_Loaded && "Vertex에러 셰이더 로드 실패");
        if (!isErrorVS_Green_Loaded)
        {
            MessageBoxA(nullptr, "CRITICAL: Error VS Green Load Failed!", "Engine Error", MB_OK);
            return false;
        }

        errorPS_Pink_ = make_shared<PixelShader>();
        bool isErrorVS_Pink_Loaded = errorPS_Pink_->initialize(device, L"resources/Unlit_Common/Error_PS_Pink.hlsl", "psMain");
        assert(isErrorVS_Pink_Loaded && "Pixel에러 셰이더 로드 실패");
        if (!isErrorVS_Pink_Loaded)
        {
            MessageBoxA(nullptr, "CRITICAL: Error VS Pink Load Failed!", "Engine Error", MB_OK);
            return false;
        }

        wireBlack_ = make_shared<PixelShader>();
        bool isWireBlack_Loaded = wireBlack_->initialize(device, L"resources/Unlit_Common/Wireframe_PS_DarkGray.hlsl", "psMain");
        assert(isWireBlack_Loaded && "wireBlack셰이더 로드 실패");
        if (!isWireBlack_Loaded) 
        {
            MessageBoxA(nullptr, "CRITICAL: wireBlack PS Load Failed!", "Engine Error", MB_OK);
            return false;
        }

        wireRed_ = make_shared<PixelShader>();
        bool isWireRed_Loaded = wireRed_->initialize(device, L"resources/Unlit_Common/Wireframe_PS_Red.hlsl", "psMain");
        assert(isWireRed_Loaded && "wireRed셰이더 로드 실패");
        if (!isWireRed_Loaded)
        {
            MessageBoxA(nullptr, "CRITICAL: wireRed PS Load Failed!", "Engine Error", MB_OK);
            return false;
        }

        default_ = make_shared<PixelShader>();
        bool isDefault_Loaded = default_->initialize(device, L"resources/Unlit_Common/Default_PS_LightGray.hlsl", "psMain");
        assert(isDefault_Loaded && "기본PS 셰이더 로드 실패");
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

        bufferData_.clear();
        propertyMap_.clear();
        uiPropertyMap_.clear();
        isDirty_ = false;
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

        if (constantBuffer_ && !bufferData_.empty())
        {
            constantBuffer_->updateRaw(context, bufferData_.data(), static_cast<UINT>(bufferData_.size()));
            constantBuffer_->bindPS(context, 2);
        }
    }

    void Material::createBuffer(ID3D11Device* device)
    {
        if (bufferData_.empty()) { return; }

        size_t byteWidth = bufferData_.size();
        size_t remainder = byteWidth % 16;
        if (remainder != 0)
        {
            size_t padding = 16 - remainder;
            bufferData_.resize(byteWidth + padding, 0);
            byteWidth += padding;
        }

        constantBuffer_ = make_shared<ConstantBuffer>();

        bool result = constantBuffer_->initializeRaw(device, static_cast<UINT>(byteWidth));
        if (!result)
        {
            MessageBoxA(nullptr, "Material ConstantBuffer Create Failed!", "Error", MB_OK);
        }

        isDirty_ = false;
    }

    void Material::useProperty(const string& name, UI::WidgetData dataType, UI::WidgetType widget, UI::WidgetSettings settings)
    {
        assert(UI::isValidWidgetSettings(widget, settings) && "위젯 타입과 설정이 맞지 않습니다.");
        uiPropertyMap_[name] = UIPropData{ dataType, widget, settings };
    }
}
