#pragma once
#include <vector>
#include <memory>

class SceneObject;

enum class SceneObjectActionType 
{
    Added,
    Removed,
    Cleared,
    Count
};

struct SceneObjectChangedEvent 
{
    //TO_DO: Hierachy만들 때 변경 내역 기록
    //SceneObjectActionType type;
    //std::vector<std::shared_ptr<SceneObject>> targets;
};
