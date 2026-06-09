#pragma once
#include <string>


enum class SceneDataType
{
    DataChanged,
    FileSaved,
    FileLoaded,
    Count
};

struct SceneDataChangedEvent
{
    SceneDataType type = SceneDataType::Count;
    std::string path = "";
    
    SceneDataChangedEvent(SceneDataType t, const std::string& p) : type(t), path(p) {}
};
