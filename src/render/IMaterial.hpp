#pragma once
#include <d3d11.h>


namespace Render 
{
    class IMaterial 
    {
    public:
        virtual ~IMaterial() = default;

        virtual void bind(ID3D11DeviceContext* context) = 0;
    };
}
