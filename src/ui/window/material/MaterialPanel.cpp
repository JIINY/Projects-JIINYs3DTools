#include "MaterialPanel.hpp"
#include <memory>
#include "MaterialContainer.hpp"
#include "MaterialConfig.hpp"

#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/MaterialPopupRequestedEvent.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;


namespace Material
{
    MaterialPanel::MaterialPanel() : matContainer_(make_unique<MaterialContainer>()), matConfig_(make_unique<MaterialConfig>()) {}
    MaterialPanel::~MaterialPanel() = default;

    bool MaterialPanel::initialize()
    {
        if (!matContainer_->initialize()) { return false; }
        if (!matConfig_->initialize()) { return false; }

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
            matContainer_->draw();
            matConfig_->draw();
        }
        ImGui::End();

        if (!isOpen)
        {
            AppEventPublisher::get().publish(MaterialPopupRequestedEvent{ false });
        }
    }
}
