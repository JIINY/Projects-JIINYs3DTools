#pragma once
#include <string>


enum class EnvDataType 
{
    Save,
    SaveAs,
    Load,
    Restore,
    New,
    Count
};

struct EnvironmentDataRequestedEvent
{
    EnvDataType type;
    std::string filepath;

    EnvironmentDataRequestedEvent(EnvDataType t, const std::string& p) : type(t), filepath(p) {}
};
