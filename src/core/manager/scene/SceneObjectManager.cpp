#include "SceneObjectManager.hpp"
#include <cassert>
#include <vector>
#include <memory>
#include "core/coordinator/scene/PassiveObjectCoordinator.hpp"
#include "../resources/MaterialManager.hpp"
#include "renderer/dx11/DX11Renderer.hpp"
#include "object/SceneObject.hpp"

#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/state/SceneObjectChangedEvent.hpp"
using namespace std;


SceneObjectManager::SceneObjectManager() : passiveCoord_(make_unique<PassiveObjectCoordinator>()) {}
SceneObjectManager::~SceneObjectManager() = default;

bool SceneObjectManager::initialize(SceneObjectContext context) 
{
    if (!passiveCoord_->initialize(context.renderer, context.matManager)) { return false; }

    auto objChangeID = AppEventSubscriber::get().subscribe<SceneObjectChangedEvent>([this](const SceneObjectChangedEvent& event)
        {
            this->isDirty_ = true;
        });
    appEventSubID_.push_back(objChangeID);

    return true;
}

void SceneObjectManager::update(float deltaTime) 
{
    passiveCoord_->update(deltaTime);
}

const vector<shared_ptr<SceneObject>>& SceneObjectManager::getAllSceneObjects() const 
{
    if (!isDirty_) { return cachedObjects_; }

    vector<shared_ptr<SceneObject>> passiveObjs;
    if (passiveCoord_) 
    {
        passiveObjs = passiveCoord_->getBaseObjects();
    }

    size_t size = passiveObjs.size();

    cachedObjects_.clear();
    cachedObjects_.reserve(size);

    cachedObjects_.insert(cachedObjects_.end(), passiveObjs.begin(), passiveObjs.end());

    isDirty_ = false;
    return cachedObjects_;
}

void SceneObjectManager::removeObject(std::shared_ptr<SceneObject> obj)
{
    if (!obj) { return; }

    switch (obj->getObjectType())
    {
    case SceneObjectType::BaseObject:
    {
        if (passiveCoord_) { passiveCoord_->removeObject(obj); }
        break;
    }
    default:
        break;
    }
    isDirty_ = true;
}

void SceneObjectManager::removeAllObjects()
{
    if (passiveCoord_) { passiveCoord_->removeAllObjects(); }
    isDirty_ = true;
}
