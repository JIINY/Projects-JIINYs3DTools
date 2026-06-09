#pragma once
#include <memory>
#include "event/appEvent/AppEventSubscriber.hpp"
#include "scene/SceneSerializer.hpp"

class MaterialManager;
struct SceneDataRequestedEvent;
class SceneObjectManager;
namespace EnvConfig { class EnvironmentConfig; }


struct SceneFileContext
{
    EnvConfig::EnvironmentConfig* envConfig = nullptr;
    SceneObjectManager* sceneObjManager = nullptr;
    MaterialManager* matManager = nullptr;
};

class SceneFileManager
{
public:
    SceneFileManager();
    ~SceneFileManager();

    bool initialize(SceneFileContext context);


private:
    std::vector<AppEventSubscriptionID> appEventSubID_;
    std::unique_ptr<Scene::SceneSerializer> sceneSerializer_;
    std::string currentFilePath_ = "";
    std::string currentFileName_ = "";
    bool isDirty_ = false;
    bool isLoading_ = false;

    void onSceneDataRequested(const SceneDataRequestedEvent& event);
    void onEnvironmentConfigChanged();
    void saveAs();
};
