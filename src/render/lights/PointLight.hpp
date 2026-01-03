#pragma once
#include "Light.hpp"


namespace Render 
{
    class PointLight : public Light 
    {
    public:
        PointLight() : Light(LightType::Point) 
        {
            data_.pos = { 0.0f, 0.0f, 0.0f };
            data_.range = { 10.0f };
            data_.color = { 0.5f, 1.0f, 0.5f };
            data_.intensity = 1.0f;
        }
    };
}
