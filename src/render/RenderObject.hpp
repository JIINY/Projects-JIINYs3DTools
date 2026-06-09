#pragma once
#include "../object/Entity.hpp"
#include "Renderable.hpp"
#include <memory>
#include <vector>
#include "Mesh.hpp"
#include "Material.hpp"
#include "ConstantBuffer.hpp"


namespace Render 
{
    struct RenderElement 
    {
        std::shared_ptr<Mesh> mesh_ = nullptr;
        std::shared_ptr<Material> material_ = nullptr;
    };

    class RenderObject : public Objects::Entity, public Renderable
    {
    public:
        virtual ~RenderObject() = default;

        void initialize(ID3D11Device* device);
        void initialize(ID3D11Device* device, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
        void initialize(ID3D11Device* device, const std::vector<RenderElement>& elements);
        virtual void draw(ID3D11DeviceContext* context, const Math::Mat4& viewMat, const Math::Mat4& projMat, 
            std::shared_ptr<PixelShader> overridePS = nullptr) override; //TODO: isDynamic에 따라 바뀔 수 있음

        void addElement(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat) { elements_.push_back({ mesh, mat }); }
        void clearElements() { elements_.clear(); }

        void setMesh(std::shared_ptr<Mesh> mesh);
        void setMaterial(std::shared_ptr<Material> mat);
        void setMaterial(size_t index, std::shared_ptr<Material> mat);
        std::shared_ptr<Mesh> getMesh(size_t index = 0) const;
        std::shared_ptr<Material> getMaterial(size_t index = 0) const;
        size_t getMaterialCount() const { return elements_.size(); }


    protected:
        void createConstantBuffer(ID3D11Device* device);
        void drawInternal(ID3D11DeviceContext* context, const Math::Mat4& viewMat, const Math::Mat4& projMat, std::shared_ptr<PixelShader> overridePS);


    private:
        std::vector<RenderElement> elements_;
        std::shared_ptr<ConstantBuffer> worldMatBuffer_ = nullptr;
    };
}
