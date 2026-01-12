#include "EnvironmentConfig.hpp"
#include <filesystem>
#include "EnvironmentConfigSerializer.hpp"
#include "common/Fonts.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/EnvironmentConfigRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentDataRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentDataChangedEvent.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;


namespace EnvConfig 
{
    void EnvironmentConfig::initialize(LightManager* manager) 
    {
        lightConfig_.initialize(manager);
        
        auto envDataChangeID = AppEventSubscriber::get().subscribe<EnvironmentDataChangedEvent>([this](const EnvironmentDataChangedEvent& event)
            {
                this->onEnvironmentDataChange();
            });
        AppEventSubID_.push_back(envDataChangeID);
    }

    void EnvironmentConfig::onEnvironmentDataChange() 
    {
        isDirty_ = false;
        lightConfig_.setFromManager();

        switch (pendingRequestType_) 
        {
        case EnvDataType::Save: 
        {
            break;
        }
        case EnvDataType::SaveAs:
        {
            break;
        }
        case EnvDataType::Load:
        {
            break;
        }
        case EnvDataType::Restore: 
        {
            break;
        }
        case EnvDataType::New:
        {
            currentFilePath_ = "";
            break;
        }
        default:
            assert(0 && "초기화된 EnvDataType이 ChangedEvent로 왔습니다.\n");
            break;
        }
    }

    void EnvironmentConfig::draw(bool isVisible, EnvConfig::EnvironmentConfigSerializer* serializer)
    {
        if (!isVisible) { return; }

        bool isOpen = true;
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_Always);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

        if (ImGui::Begin("Environment Config", &isOpen, window_flags))
        {
            if (lightConfig_.draw()) { isDirty_ = true; }

            //버튼
            ImGui::Separator();
            float currentWidth = ImGui::GetContentRegionAvail().x;
            float spacing = 2.0f;
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, 0));
            float btnWidth2 = (currentWidth - spacing) / 2.0f;
            float btnWidth3 = (currentWidth - (spacing * 2)) / 3.0f;
            float btnHeight = ImGui::GetFrameHeight();

            if (ImGui::Button("Save", ImVec2(btnWidth2, btnHeight + 1.0f)))
            {
                AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvDataType::Save, "assets/environments/test.json" });
            }
            ImGui::SameLine();
            if (ImGui::Button("Load", ImVec2(btnWidth2, btnHeight + 1.0f)))
            {
                AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvDataType::Load, "assets/environments/test.json" });
            }

            ImGui::Dummy(ImVec2(0.0f, 3.0f));

            if (ImGui::Button("Save As", ImVec2(btnWidth3, btnHeight + 2.0f)))
            {
                AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvDataType::SaveAs, "assets/environments/test.json" });
            }
            ImGui::SameLine();
            if (ImGui::Button("Restore", ImVec2(btnWidth3, btnHeight + 2.0f)))
            {
                AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvActionType::Restore, currentFileName_ });
            }
            ImGui::SameLine();
            if (ImGui::Button("New", ImVec2(btnWidth3, btnHeight + 2.0f)))
            {
                if (isDirty_)
                {
                    pendingRequestType_ = EnvActionType::New;
                    openConfirmDiscardTrigger_ = true;
                }
                else
                {
                    AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvActionType::New, ""});
                }
            }

            ImGui::Dummy(ImVec2(0.0f, 3.0f));
            if (currentFilePath_.empty()) { ImGui::Text("file: "); }
            else
            {
                string name = "";
                name = filesystem::path(currentFilePath_).filename().string();
                ImGui::Text("file: %s", name.c_str());
            }

            ImGui::PopStyleVar();


        }
        ImGui::End();


        if (openConfirmDiscardTrigger_) 
        {
            ImGui::OpenPopup(getPopupTitleFromType(pendingRequestType_));
            openConfirmDiscardTrigger_ = false;
        }
    
        ImGui::SetNextWindowSize(ImVec2(272, 116));
        if (ImGui::BeginPopupModal(getPopupTitleFromType(pendingRequestType_), NULL, ImGuiWindowFlags_NoResize))
        {

            ImGui::PushFont(Fonts::Regular18);
            ImGui::Text(utf8("작업 내용이 버려집니다.\n 진행하시겠습니까?"));
            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15, 0));

            if (ImGui::Button(utf8("예"), ImVec2(120, 0)))
            {
                if (pendingRequestType_ == EnvActionType::New)
                {
                    AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvActionType::New, ""});
                }
                else if (pendingRequestType_ == EnvActionType::Load)
                {
                    doLoad();
                }
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();
            if (ImGui::Button(utf8("아니오"), ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopFont();
            ImGui::PopStyleVar();
            ImGui::EndPopup();
        }

        if (!isOpen) 
        {
            AppEventPublisher::get().publish(EnvironmentConfigRequestedEvent{ false });
        }
    }

    const char* EnvironmentConfig::getPopupTitleFromType(const EnvActionType& type) 
    {
        switch (type) 
        {
        case EnvActionType::Save: return "Save Environment Config";
        case EnvActionType::SaveAs: return "Save As Environment Config";
        case EnvActionType::Load: return "Load Environment Config";
        case EnvActionType::New: return "New Environment Config";
        default: return "Notification";
        }
    }
}

    void EnvironmentConfig::doLoad() 
    {
        string filePath = FileDialogUtils::OpenFile("JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0");
        if (!filePath.empty())
        {
            AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvActionType::Load, filePath });
        }
    }
}
