#pragma once
#include "LightConfig.hpp"

class LightManager;


namespace EnvConfig 
{
    class EnvironmentConfig
    {
    public:
        void initialize(LightManager* manager);
        void draw(bool isVisible);


    private:
        EnvConfig::LightConfig lightConfig_;
    };
}
