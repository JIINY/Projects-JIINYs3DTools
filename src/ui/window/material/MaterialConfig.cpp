#include "MaterialConfig.hpp"
#include "imgui.h"
using namespace std;


namespace Material
{
    bool MaterialConfig::MaterialConfig::initialize()
    {
        return true;
    }

    void MaterialConfig::MaterialConfig::draw()
    {
        ImGui::Text("Config");
    }
}
