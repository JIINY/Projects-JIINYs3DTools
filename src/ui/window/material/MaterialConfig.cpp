#include "MaterialConfig.hpp"
#include <variant>
#include <type_traits>
#include <string>
#include "common/Math.hpp"
#include "object/SceneObject.hpp"
#include "../shaders/PresetInclude.hpp"
#include "core/manager/resources/MaterialManager.hpp"

#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/material/MaterialTargetChangedEvent.hpp"
#include "imgui.h"
using namespace std;


namespace MaterialEditor
{
    bool MaterialConfig::initialize(MaterialManager* matManager)
    {
        assert(matManager && "초기화 실패");
        if (!matManager) { return false; }
        matManager_ = matManager;

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

        static ImGuiTableFlags flags = ImGuiTableFlags_None;
        if (ImGui::BeginTable("ShaderTable", 2, flags))
        {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch, 0.28f);
            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 0.72f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Object");
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::Text(targetName_.c_str());

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Shader");
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-FLT_MIN);

            //wstring을 변환. ImGui는 UTF-8 char* 사용
            const wstring currentName = inactive ? L"" : targetMat_->getPreset();
            string currentShader(currentName.begin(), currentName.end());

            const float rightWidth = 200.0f;
            const float rightPadding = 0.0f;
            const float availWidth = ImGui::GetContentRegionAvail().x;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availWidth - rightWidth - rightPadding);
            ImGui::SetNextItemWidth(rightWidth);

            if (ImGui::BeginCombo("##Shader", currentShader.c_str()))
            {
                for (const auto& [shaderName, shaderSet] : presetMap)
                {
                    string itemLabel(shaderName.begin(), shaderName.end());
                    bool isCurrent = (shaderName == currentName);

                    if (ImGui::Selectable(itemLabel.c_str(), isCurrent))
                    {
                        if (!isCurrent)
                        {
                            auto newMat = matManager_->createMaterial(shaderName);
                            targetObj_->setMaterial(0, newMat);
                            targetMat_ = newMat;
                        }
                    }

                    if (isCurrent)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();

            }
            ImGui::EndTable();

            ImGui::Separator();
            if (!inactive)
            {
                if (ImGui::BeginTable("PropertyTable", 2, flags))
                {
                    ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch, 0.28f);
                    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 0.72f);

                    for (const auto& [name, uiData] : targetMat_->getUIPropertyMap())
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("%s", name.c_str());

                        ImGui::TableNextColumn();
                        ImGui::SetNextItemWidth(-FLT_MIN);

                        visit([&](auto&& settings)
                            {
                                using T = decay_t<decltype(settings)>;

                                if constexpr (is_same_v<T, UI::FloatDragData>)
                                {
                                    auto value = targetMat_->getProperty<float>(name);
                                    if (UI::drawFloatDrag(name, value, settings, true))
                                    {
                                        targetMat_->setProperty(name, value);
                                    }
                                }
                                else if constexpr (is_same_v<T, UI::FloatSliderData>)
                                {
                                    auto value = targetMat_->getProperty<float>(name);
                                    if (UI::drawFloatSlider(name, value, settings, true))
                                    {
                                        targetMat_->setProperty(name, value);
                                    }
                                }
                                else if constexpr (is_same_v<T, UI::FloatDragnSliderData>)
                                {
                                    auto value = targetMat_->getProperty<float>(name);
                                    if (UI::drawFloatDragnSlider(name, value, settings, true))
                                    {
                                        targetMat_->setProperty(name, value);
                                    }
                                }
                                else if constexpr (is_same_v<T, UI::Float3ColorData>)
                                {
                                    auto value = targetMat_->getProperty<Math::Vec3>(name);
                                    if (UI::drawFloat3Color(name, value, settings, true))
                                    {
                                        targetMat_->setProperty(name, value);
                                    }
                                }
                                else if constexpr (is_same_v<T, UI::Float4ColorData>)
                                {
                                    auto value = targetMat_->getProperty<Math::Vec4>(name);
                                    if (UI::drawFloat4Color(name, value, settings, true))
                                    {
                                        targetMat_->setProperty(name, value);
                                    }
                                }
                            }, uiData.settings);
                    }
                    ImGui::EndTable();
                }
            }
        }
        ImGui::EndDisabled();
    }

    void MaterialConfig::onMaterialTargetChanged(const MaterialTargetChangedEvent& event)
    {
        if (!event.target)
        {
            targetObj_ = nullptr;
            targetMat_ = nullptr;
            targetName_.clear();
            return;
        }

        //TO_DO: 멀티 매티리얼 도입시 편집 대상 인덱스 선택 필요. 현재는 0번 고정
        targetObj_ = event.target;
        targetMat_ = event.target->getMaterial(0);
        targetName_ = event.target->getName();
    }
}
