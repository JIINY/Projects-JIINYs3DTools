#pragma once
#include "Light.hpp"


namespace Render 
{
    class SpotLight : public Light 
    {
    public:
        SpotLight() : Light(LightType::Spot) 
        {
            data_.pos = { 0.0f, 5.0f, 0.0f };
            data_.direction = { 0.0f, -1.0f, 0.0f };
            data_.range = 20.0f;
            data_.spotAngle = 30.0f;

            data_.color = { 1.0f, 0.5f, 0.5f };
            data_.intensity = 2.0f;
        }
    };
}
