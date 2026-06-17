#pragma once
#include <string>
#include <Windows.h>
#include "manager/system/FloatingWindowConfig.hpp"


struct AppConfigData
{
    FloatingConfigData floatingConfig;
    std::string configPath = "./config.ini";
};


class AppConfig
{
public:
    static AppConfigData load() 
    {
        AppConfigData data;
        data.floatingConfig = makeDefaultFloatingConfigData();

        for (auto& w : data.floatingConfig.floatingInfo)
        {
            w.isVisible = GetPrivateProfileIntA(w.group.c_str(), w.configKey.c_str(), 0, data.configPath.c_str());
        }

        return data;
    }

    static void save(const AppConfigData& data) 
    {
        for (const auto& w : data.floatingConfig.floatingInfo)
        {
            WritePrivateProfileStringA(w.group.c_str(), w.configKey.c_str(), w.isVisible ? "1" : "0", data.configPath.c_str());
        }
    }
};
