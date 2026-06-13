#pragma once
#include "render/RenderObject.hpp"
#include "render/Material.hpp"
#include "render/Mesh.hpp"


class ColliderObject : public Render::RenderObject
{
public:
    ColliderObject(ID3D11Device* device, std::shared_ptr<Render::Mesh> mesh)
    {
        auto mat = std::make_shared<Render::Material>();
        mat->initialize(device);
        //mat->loadPixelShader(device, L"resources/Unlit_Common/Collider_PS_Green.hlsl", "psMain");

        initialize(device, mesh, mat);
        setRenderQueue(static_cast<int>(Render::RenderQueue::Collider));
    }
};
