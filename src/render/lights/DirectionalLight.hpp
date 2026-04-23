#pragma once
#include "Light.hpp"


namespace Render 
{
    class DirectionalLight : public Light 
    {
    public:
        DirectionalLight() : Light(LightType::Directional) 
        {
            data_.direction = { 0.0f, 1.0f, 0.0f };
            data_.color = { 1.0f, 1.0f, 1.0f };
        }
    };
}
