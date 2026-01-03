#pragma once
#include <string>
#include <Windows.h>


struct FloatingConfigData 
{
    bool showEnvConfig = false;
    bool showCameraInfo = false;
};

struct AppConfigData
{
    FloatingConfigData floatingConfig;
};


class AppConfig
{
public:
    static AppConfigData load() 
    {
        AppConfigData data;

        data.floatingConfig.showEnvConfig = GetPrivateProfileIntA("Window", "ShowEnvConfig", 0, "./config.ini");
        data.floatingConfig.showCameraInfo = GetPrivateProfileIntA("Pref", "ShowCameraInfo", 0, "./config.ini");

        return data;
    }

    static void save(const AppConfigData& data) 
    {
        WritePrivateProfileStringA("Window", "ShowEnvConfig", data.floatingConfig.showEnvConfig ? "1" : "0", "./config.ini");
        WritePrivateProfileStringA("Pref", "ShowCameraInfo", data.floatingConfig.showCameraInfo ? "1" : "0", "./config.ini");
    }
};
