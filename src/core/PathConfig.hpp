#pragma once
#include <string>


namespace Path
{
    inline std::string& rootPath() { static std::string s; return s; }
    inline std::string assetPath() { return rootPath() + "assets/"; }
    inline std::string resourcePath() { return rootPath() + "resources/"; }
    inline std::string shaderPath() { return rootPath() + "shaders/"; }
    inline std::string srcPath() { return rootPath() + "src/"; }
}

namespace EnvPathConfig 
{
    const std::string defaultFilePath_ = "src/defaults/DefaultEnvironmentConfig.json";
    const std::string userFilePath_ = "assets/environments/";
}

namespace ScenePathConfig
{
    const std::string defaultFilePath_ = "src/defaults/DefaultScene.json";
    const std::string userFilePath_ = "assets/scenes/";
}
