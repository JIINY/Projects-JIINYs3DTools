#include "Mesh.hpp"
#include <vector>
#include <d3d11.h>
#include <wrl/client.h>
using namespace std;


namespace Render 
{
    void Mesh::initialize(ID3D11Device* device) 
    {
        if (!vertices_.empty()) 
        {
            D3D11_BUFFER_DESC vbd = {};
            vbd.Usage = D3D11_USAGE_DEFAULT;
            vbd.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices_.size());
            vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vbd.CPUAccessFlags = 0;

            D3D11_SUBRESOURCE_DATA vInitData = {};
            vInitData.pSysMem = vertices_.data();

            device->CreateBuffer(&vbd, &vInitData, vertexBuffer_.GetAddressOf());
        }

        if (!indices_.empty())
        {
            D3D11_BUFFER_DESC ibd = {};
            ibd.Usage = D3D11_USAGE_DEFAULT;
            ibd.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * indices_.size());
            ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
            ibd.CPUAccessFlags = 0;

            D3D11_SUBRESOURCE_DATA iInitData = {};
            iInitData.pSysMem = indices_.data();

            device->CreateBuffer(&ibd, &iInitData, indexBuffer_.GetAddressOf());
        }
    }

    void Mesh::draw(ID3D11DeviceContext* context) const 
    {
        if (!vertexBuffer_ || !indexBuffer_) 
        {
            return;
        }

        //정점 버퍼 바인딩
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, vertexBuffer_.GetAddressOf(), &stride, &offset);

        context->IASetIndexBuffer(indexBuffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

        //프리미티브 토폴로지 설정(삼각형 리스트)
        context->IASetPrimitiveTopology(topology_);

        context->DrawIndexed(static_cast<UINT>(indices_.size()), 0, 0);
    }
}
