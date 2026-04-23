#include "ConstantBuffer.hpp"
#include <iostream>
using namespace std;


namespace Render 
{
    bool ConstantBuffer::initializeRaw(ID3D11Device* device, UINT byteWidth) 
    {
        //DirectX의 제약사항: 상수 버퍼 크기는 16byte배수일 것
        UINT alignedWidth = (byteWidth + 15) & ~15;

        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DYNAMIC; //CPU가 자주 내용을 바꿈(동적)
        bd.ByteWidth = alignedWidth;
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hr = device->CreateBuffer(&bd, nullptr, buffer_.GetAddressOf());
        if (FAILED(hr)) 
        {
            cout << "ConstantBuffer Create Failed" << endl;
            return false;
        }

        return true;
    }

    void ConstantBuffer::updateRaw(ID3D11DeviceContext* context, const void* data, UINT byteWidth) 
    {
        if (!buffer_) { return; }

        D3D11_MAPPED_SUBRESOURCE mappedResource; //GPU메모리를 잠시 CPU가 쓸 수 있게 잠금(MAP)
        HRESULT hr = context->Map(buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource); //WRITE_DISCARD 덮어쓰기 (=가장 빠름)
        if (SUCCEEDED(hr)) 
        {
            memcpy(mappedResource.pData, data, byteWidth);
            context->Unmap(buffer_.Get(), 0);
        }
    }

    void ConstantBuffer::bindVS(ID3D11DeviceContext* context, UINT slot) const
    {
        ID3D11Buffer* buffer = buffer_.Get();
        context->VSSetConstantBuffers(slot, 1, &buffer); //슬롯번호, 개수, 버퍼주소
    }

    void ConstantBuffer::bindPS(ID3D11DeviceContext* context, UINT slot) const
    {
        ID3D11Buffer* buffer = buffer_.Get();
        context->PSSetConstantBuffers(slot, 1, &buffer);
    }

}
