#pragma once
#include "render/Material.hpp"
#include "common/Math.hpp"


namespace Render::Materials
{
    class UnlitMaterial : public Material
    {
    public:
        virtual ~UnlitMaterial() = default;

        bool initialize(ID3D11Device* device, const std::wstring& vsPath, const std::wstring& psPath);

        void setBaseColor(const Math::Vec4& color);
    };
}
