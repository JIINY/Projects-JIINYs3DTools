#include "EnvironmentConfig.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/ui/EnvironmentConfigRequestedEvent.hpp"
#include "imgui.h"
using namespace std;


namespace EnvConfig 
{
    void EnvironmentConfig::initialize(LightManager* manager) 
    {
        lightConfig_.initialize(manager);
    }

    void EnvironmentConfig::draw(bool isVisible) 
    {
        if (!isVisible) { return; }
        
        bool isOpen = true;
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Environment Config", &isOpen)) 
        {
            lightConfig_.draw();
        }
        ImGui::End();

        if (!isOpen) 
        {
            AppEventPublisher::get().publish(EnvironmentConfigRequestedEvent{ false });
        }
    }
}
