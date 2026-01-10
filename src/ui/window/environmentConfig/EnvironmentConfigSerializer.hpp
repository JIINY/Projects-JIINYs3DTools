#pragma once
#include <string>
#include <memory>
#include "../lib/nlohmann-json/json.hpp"

class LightManager;


namespace EnvConfig 
{
    struct EnvConfigContext
    {
        LightManager* lightManager = nullptr;
    };


    class EnvironmentConfigSerializer
    {
    public:
        void initialize(EnvConfigContext context);
        bool serialize(const std::string& filepath);
        bool deserialize(const std::string& filepath);

    private:
        LightManager* lightManager_ = nullptr;
    };
}
