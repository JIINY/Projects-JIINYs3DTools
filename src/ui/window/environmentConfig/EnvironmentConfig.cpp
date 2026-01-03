#include "EnvironmentConfig.hpp"
#include "EnvironmentConfigSerializer.hpp"
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

    void EnvironmentConfig::draw(bool isVisible, EnvConfig::EnvironmentConfigSerializer* serializer) 
    {
        if (!isVisible) { return; }
        
        bool isOpen = true;
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Environment Config", &isOpen)) 
        {
            lightConfig_.draw();

            //버튼
            ImGui::Separator();
            float currentWidth = ImGui::GetContentRegionAvail().x;
            float spacing = 2.0f;
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, 0));
            float btnWidth = (currentWidth - (spacing * 2)) / 3.0f;

            if (ImGui::Button("Save", ImVec2(btnWidth, 0))) 
            {
                if (serializer) { serializer->serialize("assets/environments/test.json"); }
            }
            ImGui::SameLine();
            if (ImGui::Button("Load", ImVec2(btnWidth, 0)))
            {
                if (serializer) { serializer->deserialize("assets/environments/test.json"); }
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset", ImVec2(btnWidth, 0)))
            {
                if (serializer) { serializer->deserialize("assets/environments/DefaultConfig.json"); }
            }

            ImGui::PopStyleVar();
        }
        ImGui::End();

        if (!isOpen) 
        {
            AppEventPublisher::get().publish(EnvironmentConfigRequestedEvent{ false });
        }
    }
}
