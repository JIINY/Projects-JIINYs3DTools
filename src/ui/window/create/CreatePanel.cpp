#include "CreatePanel.hpp"
#include <memory>
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/CreatePopupRequestedEvent.hpp"
#include "CreatePrimitive.hpp"
#include "command/CommandStack.hpp"
#include "core/coordinator/scene/PassiveObjectCoordinator.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;


namespace Create 
{
    bool CreatePanel::initialize(PassiveObjectCoordinator* passiveObjCoord) 
    {
        if (!primitive_.initialize(passiveObjCoord)) { return false; }
        return true;
    }

    void CreatePanel::draw(bool isVisible)
    {
        if (!isVisible) { return; }

        bool isOpen = true;
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_Always);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;
        if (ImGui::Begin("Create", &isOpen, window_flags))
        {
            primitive_.draw();
        }
        ImGui::End();

        if (!isOpen)
        {
            AppEventPublisher::get().publish(CreatePopupRequestedEvent{ false });
        }
    }
}
