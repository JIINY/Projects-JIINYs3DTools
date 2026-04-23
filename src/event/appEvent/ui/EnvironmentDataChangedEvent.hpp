#pragma once
#include <string>


enum class EnvDataType 
{
    DataChanged,
    FileSaved,
    FileLoaded    
};

struct EnvironmentDataChangedEvent 
{
    EnvDataType type = EnvDataType::DataChanged;
    std::string path = "";

    EnvironmentDataChangedEvent(EnvDataType t = EnvDataType::DataChanged, const std::string& p = "")
        : type(t), path(p) {}
};
