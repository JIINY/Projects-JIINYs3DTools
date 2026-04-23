#include "RenderObject.hpp"
#include <vector>
#include <memory>
#include "Mesh.hpp"
#include "Material.hpp"
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
        DebugLog("[Enter] this: %p, override: %s, mat _11, _12, _13, _14: %.3f, %.3f, %.3f, %.3f",
            this, overridePS ? "WIRE" : "OPAQUE", m._11, m._12, m._13, m._14);
        DebugLog("[Enter] this: %p, override: %s, mat _21, _22, _23, _24: %.3f, %.3f, %.3f, %.3f",
            this, overridePS ? "WIRE" : "OPAQUE", m._21, m._22, m._23, m._24);
        DebugLog("[Enter] this: %p, override: %s, mat _31, _32, _33, _34: %.3f, %.3f, %.3f, %.3f",
            this, overridePS ? "WIRE" : "OPAQUE", m._31, m._32, m._33, m._34);
        DebugLog("[Enter] this: %p, override: %s, mat _41, _42, _43, _44: %.3f, %.3f, %.3f, %.3f",
            this, overridePS ? "WIRE" : "OPAQUE", m._41, m._42, m._43, m._44);


        Render::WorldMatConstantBufferData worldMatData;

        //DirectXMath행렬을 HLSL로 보낼 때 Transpose(전치)가 필요한 경우가 많음: Row-Major -> Column-Major 변환
        auto transpose = [](const Math::Mat4& m) {
            XMMATRIX mat = XMLoadFloat4x4(&m);
            Math::Mat4 res;
            XMStoreFloat4x4(&res, XMMatrixTranspose(mat));
            return res;
            };

        worldMatData.world = getWorldMatForShader();

        //업로드 직전
        DebugLog("[Upload] this: %p, override: %s, trans  _11, _12, _13, _14: %.3f, %.3f, %.3f, %.3f",
            this, overridePS ? "WIRE" : "OPAQUE", worldMatData.world._11, worldMatData.world._12, worldMatData.world._13, worldMatData.world._14);
        DebugLog("[Upload] this: %p, override: %s, trans  _21, _22, _23, _24: %.3f, %.3f, %.3f, %.3f",
            this, overridePS ? "WIRE" : "OPAQUE", worldMatData.world._21, worldMatData.world._22, worldMatData.world._23, worldMatData.world._24);
        DebugLog("[Upload] this: %p, override: %s, trans  _31, _32, _33, _34: %.3f, %.3f, %.3f, %.3f",
            this, overridePS ? "WIRE" : "OPAQUE", worldMatData.world._31, worldMatData.world._32, worldMatData.world._33, worldMatData.world._34);
        DebugLog("[Upload] this: %p, override: %s, trans  _41, _42, _43, _44: %.3f, %.3f, %.3f, %.3f",
            this, overridePS ? "WIRE" : "OPAQUE", worldMatData.world._41, worldMatData.world._42, worldMatData.world._43, worldMatData.world._44);

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
