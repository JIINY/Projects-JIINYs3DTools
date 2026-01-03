#include "RenderObject.hpp"
#include "structures/WorldMatConstantBufferData.hpp"
using namespace std;
using namespace DirectX;


namespace Render 
{
    void RenderObject::initialize(ID3D11Device* device, shared_ptr<Mesh> mesh, shared_ptr<Material> material) 
    {
        mesh_ = mesh;
        material_ = material;

        worldMatBuffer_ = make_shared<ConstantBuffer>();
        worldMatBuffer_->initialize<Render::WorldMatConstantBufferData>(device);
    }

    void RenderObject::draw(ID3D11DeviceContext* context, const Math::Mat4& viewMat, const Math::Mat4& projMat, shared_ptr<PixelShader> overridePS) 
    {
        if (!mesh_ || !material_ || !worldMatBuffer_) { return; }
        if (!isVisible_) { return; }

        Render::WorldMatConstantBufferData worldMatData;

        //DirectXMath행렬을 HLSL로 보낼 때 Transpose(전치)가 필요한 경우가 많음: Row-Major -> Column-Major 변환
        auto transpose = [](const Math::Mat4& m) {
            XMMATRIX mat = XMLoadFloat4x4(&m);
            Math::Mat4 res;
            XMStoreFloat4x4(&res, XMMatrixTranspose(mat));
            return res;
        };

        worldMatData.world = transpose(getWorldMatrix());
        worldMatBuffer_->update(context, worldMatData);

        //VS에 Transform Buffer(b1) 연결
        ID3D11Buffer* cb = worldMatBuffer_->getBuffer();
        worldMatBuffer_->bindVS(context, 1);

        //재질 적용
        material_->bind(context);
        if (overridePS) 
        {
            overridePS->bind(context); //wire
        }

        mesh_->draw(context);
    }
}
