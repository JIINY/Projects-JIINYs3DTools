#pragma once
#include <memory>

class SceneObject;


struct MaterialTargetChangedEvent
{
    std::shared_ptr<SceneObject> target;
};
