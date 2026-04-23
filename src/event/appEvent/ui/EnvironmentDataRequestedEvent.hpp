#pragma once
#include <string>


enum class EnvActionType 
{
    Save,
    SaveAs,
    Load,
    Restore,
    New,
    UIModify,
    Count
};

struct EnvironmentDataRequestedEvent
{
    EnvActionType type = EnvActionType::Count;
    std::string path = "";

    EnvironmentDataRequestedEvent(EnvActionType t, const std::string& p) : type(t), path(p) {}
};
