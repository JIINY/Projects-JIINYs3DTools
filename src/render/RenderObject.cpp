#include "RenderObject.hpp"
#include <vector>
#include <memory>
#include "Mesh.hpp"
#include "Material.hpp"
#include "Renderable.hpp"
#include "structures/WorldMatConstantBufferData.hpp"

#include "common/DebugLog.hpp"
using namespace std;
using namespace DirectX;


namespace Render
{
    void RenderObject::createConstantBuffer(ID3D11Device* device)
    {
        if (!worldMatBuffer_)
        {
            worldMatBuffer_ = make_shared<ConstantBuffer>();
            worldMatBuffer_->initialize<Render::WorldMatConstantBufferData>(device);
        }
    }

    void RenderObject::initialize(ID3D11Device* device)
    {
        createConstantBuffer(device);
        elements_.clear();
    }

    void RenderObject::initialize(ID3D11Device* device, shared_ptr<Mesh> mesh, shared_ptr<Material> material)
    {
        createConstantBuffer(device);
        elements_.clear();

        if (mesh && material)
        {
            elements_.push_back({ mesh, material });
        }
    }

    void RenderObject::initialize(ID3D11Device* device, const std::vector<Render::RenderElement>& elements)
    {
        createConstantBuffer(device);
        elements_ = elements;
    }

    void RenderObject::draw(ID3D11DeviceContext* context, const Math::Mat4& viewMat, const Math::Mat4& projMat, shared_ptr<PixelShader> overridePS)
    {
        drawInternal(context, viewMat, projMat, overridePS);
    }

    void RenderObject::drawInternal(ID3D11DeviceContext* context, const Math::Mat4& viewMat, const Math::Mat4& projMat, shared_ptr<PixelShader> overridePS)
    {
        if (!isVisible_ || !worldMatBuffer_ || elements_.empty()) { return; }

        const Math::Mat4& m = getWorldMatForShader();
        Render::WorldMatConstantBufferData worldMatData;

        //DirectXMath행렬을 HLSL로 보낼 때 Transpose(전치)가 필요한 경우가 많음: Row-Major -> Column-Major 변환
        auto transpose = [](const Math::Mat4& m) {
            XMMATRIX mat = XMLoadFloat4x4(&m);
            Math::Mat4 res;
            XMStoreFloat4x4(&res, XMMatrixTranspose(mat));
            return res;
            };

        worldMatData.world = getWorldMatForShader();

        worldMatBuffer_->update(context, worldMatData);
        worldMatBuffer_->bindVS(context, 1);

        for (const auto& e : elements_)
        {
            if (!e.mesh_ || !e.material_) { continue; }

            //재질 적용
            if (e.material_->getVertexShader())
            {
                e.material_->getVertexShader()->bind(context);
            }

            if (overridePS)
            {
                overridePS->bind(context); //e.g. wire
            }
            else
            {
                if (e.material_->getPixelShader())
                {
                    e.material_->getPixelShader()->bind(context);
                }
            }

            auto cb = e.material_->getConstantBuffer();
            auto& bufferData = e.material_->getBufferData();
            if (cb && !bufferData.empty())
            {
                cb->updateRaw(context, bufferData.data(), static_cast<UINT>(bufferData.size()));
                cb->bindPS(context, 2);
            }

            e.mesh_->draw(context);
        }
    }

    void RenderObject::setMesh(std::shared_ptr<Mesh> mesh) 
    {
        if (elements_.empty()) { elements_.resize(1); }
        elements_[0].mesh_ = mesh;
    }

    void RenderObject::setMaterial(std::shared_ptr<Material> mat)
    {
        if (elements_.empty()) { elements_.resize(1); }
        elements_[0].material_ = mat;
    }

    void RenderObject::setMaterial(size_t index, shared_ptr<Material> mat)
    {
        if (index < elements_.size()) 
        {
            elements_[index].material_ = mat;
        }
    }

    shared_ptr<Mesh> RenderObject::getMesh(size_t index) const
    {
        return (index < elements_.size()) ? elements_[index].mesh_ : nullptr;
    }

    shared_ptr<Material> RenderObject::getMaterial(size_t index) const
    {
        return (index < elements_.size()) ? elements_[index].material_ : nullptr;
    }
}
