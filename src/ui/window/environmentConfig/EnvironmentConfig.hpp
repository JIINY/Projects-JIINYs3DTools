#pragma once
#include "LightConfig.hpp"

class LightManager;
namespace EnvConfig
{
    class EnvironmentConfigSerializer;
}


namespace EnvConfig 
{
    class EnvironmentConfig
    {
    public:
        void initialize(LightManager* manager);
        void draw(bool isVisible, EnvConfig::EnvironmentConfigSerializer* serializer);


    private:
        EnvConfig::LightConfig lightConfig_;
    };
}
