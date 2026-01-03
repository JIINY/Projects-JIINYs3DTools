#pragma once
#include "Light.hpp"


namespace Render 
{
    class DirectionLight : public Light 
    {
    public:
        DirectionLight() : Light(LightType::Directional) 
        {
            data_.direction = { 0.0f, 1.0f, 0.0f };
            data_.color = { 1.0f, 1.0f, 1.0f };
        }
    };
}
