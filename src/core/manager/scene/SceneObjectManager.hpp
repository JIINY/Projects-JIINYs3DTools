#pragma once
#include <vector>
#include <memory>
#include "event/appEvent/AppEventSubscriber.hpp"

class DX11Renderer;
class SceneObject;
class PassiveObjectCoordinator;
class MaterialManager;


struct SceneObjectContext
{
    DX11Renderer* renderer = nullptr;
    MaterialManager* matManager = nullptr;
};

class SceneObjectManager 
{
public:
    SceneObjectManager();
    ~SceneObjectManager();

    bool initialize(SceneObjectContext context);
    void update(float deltaTime);

    void removeObject(std::shared_ptr<SceneObject> obj);


    const std::vector<std::shared_ptr<SceneObject>>& getAllSceneObjects() const;
    PassiveObjectCoordinator* getPassiveCoordinator() const { return passiveCoord_.get(); }


private:
    std::vector<AppEventSubscriptionID> appEventSubID_;
    std::unique_ptr<PassiveObjectCoordinator> passiveCoord_;
    mutable std::vector<std::shared_ptr<SceneObject>> cachedObjects_;
    mutable bool isDirty_ = false;
};
