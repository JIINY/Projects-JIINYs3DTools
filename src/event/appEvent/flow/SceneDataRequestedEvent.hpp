#pragma once
#include <string>

#include "common/DebugLog.hpp"

enum class SceneActionType
{
    Save,
    SaveAs,
    Load,
    New,
    Count
};

struct SceneDataRequestedEvent
{
    SceneActionType type = SceneActionType::Count;
    std::string path = "";
    
    SceneDataRequestedEvent(SceneActionType t, const std::string& p) : type(t), path(p) {}
};
