#pragma once
#include <string>


struct FloatingWindowInfo
{
    std::string group;
    std::string configKey;
    std::string windowName;
    bool isVisible = false;
};

struct FloatingConfigData
{
    std::vector<FloatingWindowInfo> floatingInfo;
};

inline FloatingConfigData makeDefaultFloatingConfigData()
{
    FloatingConfigData data;
    data.floatingInfo = 
    {
        //Group, configKey, windowName, isVisible
        {"Window", "showCreate", "Create", false},
        {"Window", "showEnvConfig", "Environment Config", false},
        {"Window", "showMaterialEditor", "Material", false},
        {"pref", "showCamInfo", "Camera Info", false}
    };

    return data;
}
