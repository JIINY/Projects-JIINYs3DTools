#include "UnlitMaterial.hpp"
#include "common/Math.hpp"
using namespace std;
using namespace Math;


namespace Render 
{
    namespace Materials 
    {
        UnlitMaterial::UnlitMaterial() {}
        UnlitMaterial::~UnlitMaterial() {}

        bool UnlitMaterial::initialize(ID3D11Device* device, const wstring& vsPath, const wstring& psPath) 
        {
            Material::initialize(device);

            loadVertexShader(device, vsPath, "vsMain");
            loadPixelShader(device, psPath, "psMain");

            createBuffer<Data>(device);
            return true;
        }

        void UnlitMaterial::setBaseColor(const Vec4& color) 
        {
            data_.baseColor = color;
        }

        void UnlitMaterial::bind(ID3D11DeviceContext* context) 
        {
            //CPU데이터를 GPU버퍼로 전송
            updateData(context, data_); 

            //쉐이더 바인딩(부모 호출)
            Material::bind(context);

            //버퍼를 픽셀 쉐이더의 b1슬롯에 연결
            if (constantBuffer_ && pixelShader_) 
            {
                pixelShader_->setConstantBuffer(1, Render::ShaderStage::Pixel, constantBuffer_, context);
            }
        }
    }
}
