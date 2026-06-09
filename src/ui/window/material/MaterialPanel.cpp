#include "MaterialPanel.hpp"
#include <memory>
#include "MaterialContainer.hpp"
#include "MaterialConfig.hpp"
#include "object/SceneObject.hpp"

#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/ui/MaterialPopupRequestedEvent.hpp"
#include "event/editorEvent/EditorEventSubscriber.hpp"
#include "event/editorEvent/selection/SelectionChangeEvent.hpp"
#include "event/editorEvent/selection/SelectionStateEvent.hpp"
#include "event/uiEvent/UIEventPublisher.hpp"
#include "event/uiEvent/material/MaterialTargetChangedEvent.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;


namespace MaterialEditor
{
    MaterialPanel::MaterialPanel() : matContainer_(make_unique<MaterialContainer>()), matConfig_(make_unique<MaterialConfig>()) {}
    MaterialPanel::~MaterialPanel() = default;

    bool MaterialPanel::initialize(MaterialManager* matManager)
    {
        if (!matContainer_->initialize()) { return false; }
        if (!matConfig_->initialize(matManager)) { return false; }

        auto selChangedID = EditorEventSubscriber::get().subscribe<SelectionChangedEvent>([this](const SelectionChangedEvent& event)
            {
                this->onSelectionChanged(event);
            });
        editorEventSubID_.push_back(selChangedID);

        auto selProvidedID = EditorEventSubscriber::get().subscribe<SelectionStateProvidedEvent>([this](const SelectionStateProvidedEvent& event)
            {
                this->onSelectionStateProvided(event);
            });
        editorEventSubID_.push_back(selProvidedID);

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
            ImGui::Separator();
            matConfig_->draw();
        }
        ImGui::End();

        if (!isOpen)
        {
            AppEventPublisher::get().publish(MaterialPopupRequestedEvent{ false });
        }
    }

    shared_ptr<SceneObject> MaterialPanel::convertSceneObject(const shared_ptr<Selection::Selectable>& sel) const
    {
        return dynamic_pointer_cast<SceneObject>(sel);
    }

    void MaterialPanel::applySelection(const vector<shared_ptr<Selection::Selectable>>& selection)
    {
        currentSelectionSize_ = static_cast<int>(selection.size());
        lastSelected_ = selection.empty() ? nullptr : convertSceneObject(selection.back());

        UIEventPublisher::get().publish(MaterialTargetChangedEvent{ lastSelected_ });
    }

    void MaterialPanel::onSelectionChanged(const SelectionChangedEvent& event)
    {
        applySelection(event.currentSelection);
    }

    void MaterialPanel::onSelectionStateProvided(const SelectionStateProvidedEvent& event)
    {
        applySelection(event.currentSelection);
    }
}
