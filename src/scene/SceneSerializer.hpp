#pragma once
#include <string>
#include <memory>
#include "object/SceneObject.hpp"
#include "../lib/nlohmann-json/json.hpp"

class PassiveObjectCoordinator;
class SceneObjectManager;
class MaterialManager;
namespace EnvConfig { class EnvironmentConfig; }


namespace Scene
{
    struct SceneContext
    {
        SceneObjectManager* sceneObjManager = nullptr;
        MaterialManager* matManager = nullptr;
        EnvConfig::EnvironmentConfig* envConfig = nullptr;
    };

    class SceneSerializer
    {
    public:
        bool initialize(SceneContext context);

        bool serialize(const std::string& filepath);
        bool deserialize(const std::string& filepath, std::string& errorMsg);


    private:
        SceneObjectManager* sceneObjectManager_ = nullptr;
        PassiveObjectCoordinator* passiveObjCoord_ = nullptr;
        MaterialManager* matManager_ = nullptr;
        EnvConfig::EnvironmentConfig* envConfig_ = nullptr;
        std::string errorMsg_ = "";

        void loadEnvConfig(const nlohmann::json& envJson, std::string& errorMsg);
        std::shared_ptr<SceneObject> loadObject(const nlohmann::json& objJson, std::string& errorMsg);
        void loadMaterial(const nlohmann::json& matJson, std::shared_ptr<SceneObject> obj, size_t index, std::string& errorMsg);
    };
}
