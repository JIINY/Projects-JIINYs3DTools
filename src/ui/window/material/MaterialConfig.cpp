#include "MaterialConfig.hpp"
#include <variant>
#include <type_traits>
#include <string>
#include "common/Math.hpp"
#include "object/SceneObject.hpp"
#include "../shaders/PresetInclude.hpp"
#include "core/manager/resources/MaterialManager.hpp"
#include "utils/FontUtils.hpp"

#include "command/CommandStack.hpp"
#include "command/material/CmdChangeShader.hpp"
#include "command/material/CmdEditProperties.hpp"
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
        if (targetObj_) { targetMat_ = targetObj_->getMaterial(matIndex_); }

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
                            CommandStack::get().execute(make_shared<CmdChangeShader>(matManager_, targetObj_, matIndex_, targetMat_->getPreset(),
                                targetMat_->getPropertyMap(), targetMat_->getUIPropertyMap(), targetMat_->getBufferData(), shaderName));
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
                                    bool activated = false;
                                    bool deactivated = false;
                                    auto value = targetMat_->getProperty<float>(name);

                                    if (UI::drawFloatDrag(name, value, settings, true, &activated, &deactivated)) //드래그 중 시각 피드백
                                    {
                                        targetMat_->setProperty(name, value);
                                    }
                                    if (activated)
                                    {
                                        dragBeforeBufferData_ = targetMat_->getBufferData();
                                    }
                                    if (deactivated)
                                    {
                                        CommandStack::get().execute(make_shared<CmdEditProperties>(targetMat_, targetMat_->getPropertyMap(),
                                            targetMat_->getUIPropertyMap(), dragBeforeBufferData_, targetMat_->getBufferData()));
                                    }
                                }
                                else if constexpr (is_same_v<T, UI::FloatSliderData>)
                                {
                                    bool activated = false;
                                    bool deactivated = false;
                                    auto value = targetMat_->getProperty<float>(name);

                                    if (UI::drawFloatSlider(name, value, settings, true, &activated, &deactivated))
                                    {
                                        targetMat_->setProperty(name, value);
                                    }
                                    if (activated)
                                    {
                                        dragBeforeBufferData_ = targetMat_->getBufferData();
                                    }
                                    if (deactivated)
                                    {
                                        CommandStack::get().execute(make_shared<CmdEditProperties>(targetMat_, targetMat_->getPropertyMap(),
                                            targetMat_->getUIPropertyMap(), dragBeforeBufferData_, targetMat_->getBufferData()));
                                    }
                                }
                                else if constexpr (is_same_v<T, UI::FloatDragnSliderData>)
                                {
                                    bool activated = false;
                                    bool deactivated = false;
                                    auto value = targetMat_->getProperty<float>(name);

                                    if (UI::drawFloatDragnSlider(name, value, settings, true, &activated, &deactivated))
                                    {
                                        targetMat_->setProperty(name, value);
                                    }
                                    if (activated)
                                    {
                                        dragBeforeBufferData_ = targetMat_->getBufferData();
                                    }
                                    if (deactivated)
                                    {
                                        CommandStack::get().execute(make_shared<CmdEditProperties>(targetMat_, targetMat_->getPropertyMap(),
                                            targetMat_->getUIPropertyMap(), dragBeforeBufferData_, targetMat_->getBufferData()));
                                    }
                                }
                                else if constexpr (is_same_v<T, UI::Float3ColorData>)
                                {
                                    bool activated = false;
                                    bool deactivated = false;
                                    auto value = targetMat_->getProperty<Math::Vec3>(name);

                                    if (UI::drawFloat3Color(name, value, settings, true, &activated, &deactivated))
                                    {
                                        targetMat_->setProperty(name, value);
                                    }
                                    if (activated)
                                    {
                                        dragBeforeBufferData_ = targetMat_->getBufferData();
                                    }
                                    if (deactivated)
                                    {
                                        CommandStack::get().execute(make_shared<CmdEditProperties>(targetMat_, targetMat_->getPropertyMap(),
                                            targetMat_->getUIPropertyMap(), dragBeforeBufferData_, targetMat_->getBufferData()));
                                    }
                                }
                                else if constexpr (is_same_v<T, UI::Float4ColorData>)
                                {
                                    bool activated = false;
                                    bool deactivated = false;
                                    auto value = targetMat_->getProperty<Math::Vec4>(name);

                                    if (UI::drawFloat4Color(name, value, settings, true, &activated, &deactivated))
                                    {
                                        targetMat_->setProperty(name, value);
                                    }
                                    if (activated)
                                    {
                                        dragBeforeBufferData_ = targetMat_->getBufferData();
                                    }
                                    if (deactivated)
                                    {
                                        CommandStack::get().execute(make_shared<CmdEditProperties>(targetMat_, targetMat_->getPropertyMap(),
                                            targetMat_->getUIPropertyMap(), dragBeforeBufferData_, targetMat_->getBufferData()));
                                    }
                                }
                            }, uiData.settings);
                    }
                    ImGui::EndTable();
                    ImGui::Dummy(ImVec2(0.1f, 0.2f));

                    float buttonWidth = 150.0f;
                    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
                    if (ImGui::Button(utf8("매티리얼 초기화"), ImVec2(buttonWidth, 0)))
                    {
                        auto beforeBufferData = targetMat_->getBufferData();
                        auto defaultMat = matManager_->createMaterial(targetMat_->getPreset());
                        CommandStack::get().execute(make_shared<CmdEditProperties>(targetMat_, targetMat_->getPropertyMap(), targetMat_->getUIPropertyMap(),
                            beforeBufferData, defaultMat->getBufferData()));
                    }
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
