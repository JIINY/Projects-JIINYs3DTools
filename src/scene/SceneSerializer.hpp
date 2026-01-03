#pragma once
#include <string>
#include <memory>
#include "../lib/nlohmann-json/json.hpp"


namespace Scene
{
    struct SceneContext
    {
    };


    class SceneSerializer
    {
    public:
        void initialize(SceneContext context);
        void serialize(const std::string& filepath);
        bool deserialize(const std::string& filepath);

    private:
    };
}
