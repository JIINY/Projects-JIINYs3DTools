#pragma once
#include "../src/render/Material.hpp"


namespace Render 
{
    namespace Materials 
    {
        class BlinnPhongMaterial : public Material
        {
        public:
            virtual ~BlinnPhongMaterial() = default;

            void initialize(ID3D11Device* device) override;
            void bind(ID3D11DeviceContext* context) override;

            void setBaseColor(float r, float g, float b, float a = 1.0f) { data_.color = { r, g, b, a }; }
            void setRoughness(float r) { data_.roughness = r; }

        private:
            MaterialData data_;
        };
    }
}
