#include "MaterialConfig.hpp"
#include <string>
#include "object/SceneObject.hpp"

#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/material/MaterialTargetChangedEvent.hpp"
#include "imgui.h"
using namespace std;


namespace MaterialEditor
{
    bool MaterialConfig::initialize()
    {
        auto targetMatID = UIEventSubscriber::get().subscribe<MaterialTargetChangedEvent>([this](const MaterialTargetChangedEvent& event)
            {
                this->onMaterialTargetChanged(event);
            });
        uiEventSubID_.push_back(targetMatID);

        return true;
    }

    void MaterialConfig::draw()
    {
        const bool inactive = (targetMat_ == nullptr);
        ImGui::BeginDisabled(inactive);

        ImGui::Text("%s", targetName_.c_str());;

        ImGui::EndDisabled();
    }

    void MaterialConfig::onMaterialTargetChanged(const MaterialTargetChangedEvent& event)
    {
        if (!event.target)
        {
            targetMat_ = nullptr;
            targetName_.clear();
            return;
        }

        //TO_DO: 멀티 매티리얼 도입시 편집 대상 인덱스 선택 필요. 현재는 0번 고정
        targetMat_ = event.target->getMaterial(0);
        targetName_ = event.target->getName();
    }
}
