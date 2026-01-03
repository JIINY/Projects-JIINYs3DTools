#pragma once
#include "../object/Entity.hpp"
#include <memory>
#include "Mesh.hpp"
#include "Material.hpp"
#include "ConstantBuffer.hpp"


namespace Render 
{
    class RenderObject : public Objects::Entity 
    {
    public:
        virtual ~RenderObject() = default;

        void initialize(ID3D11Device* device, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
        void draw(ID3D11DeviceContext* context, const Math::Mat4& viewMat, const Math::Mat4& projMat, std::shared_ptr<PixelShader> overridePS = nullptr); //TODO: isDynamic에 따라 바뀔 수 있음

        void setMesh(std::shared_ptr<Mesh> mesh) { mesh_ = mesh; }
        void setMaterial(std::shared_ptr<Material> material) { material_ = material; }
        std::shared_ptr<Mesh> getMesh() { return mesh_; }
        std::shared_ptr<Material> getMaterial() { return material_; }

        void setVisible(bool visible) { isVisible_ = visible; }
        bool isVisible() const { return isVisible_; }

        void setDynamic(bool isDynamic) { isDynamic_ = isDynamic; }
        bool isDynamic() const { return isDynamic_; }

    private:
        std::shared_ptr<Mesh> mesh_ = nullptr;
        std::shared_ptr<Material> material_ = nullptr;
        std::shared_ptr<ConstantBuffer> worldMatBuffer_ = nullptr;

        bool isVisible_ = true;
        bool isDynamic_ = false;
    };
}
