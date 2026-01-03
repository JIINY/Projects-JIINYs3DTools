#include "SceneSerializer.hpp"
#include <fstream>
#include <cassert>
#include "../lib/nlohmann-json/json.hpp"
using namespace std;
using json = nlohmann::json;


namespace Scene 
{
    void SceneSerializer::initialize(SceneContext context)
    {
    }

    void SceneSerializer::serialize(const std::string& filepath) 
    {
    }

    bool SceneSerializer::deserialize(const std::string& filepath) 
    {
        return true;
    }

}