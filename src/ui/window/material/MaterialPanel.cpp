#include "MaterialPanel.hpp"
#include <memory>
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/MaterialPopupRequestedEvent.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;


namespace Material
{
    bool MaterialPanel::initialize()
    {
        return true;
    }

    void MaterialPanel::draw(bool isVisible)
    {
        if (!isVisible) { return; }

        bool isOpen = true;
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_Always);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;
        if (ImGui::Begin("Material", &isOpen, window_flags))
        {

        }
        ImGui::End();

        if (!isOpen)
        {
            AppEventPublisher::get().publish(MaterialPopupRequestedEvent{ false });
        }
    }
}
