#pragma once
#include "render/Material.hpp"
#include "common/Math.hpp"


namespace Render 
{
    namespace Materials 
    {
        class UnlitMaterial : public Material 
        {
        public:
            struct Data 
            {
                Math::Vec4 baseColor = Math::Vec4(1.0f, 1.0f, 1.0f, 1.0f);
            };

            UnlitMaterial();
            virtual ~UnlitMaterial();

            bool initialize(ID3D11Device* device, const std::wstring& vsPath, const std::wstring& psPath);
            
            void setBaseColor(const Math::Vec4& color);
            void bind(ID3D11DeviceContext* context) override;

        private:
            Data data_{};
        };
    }
}