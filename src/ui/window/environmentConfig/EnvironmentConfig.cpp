#include "EnvironmentConfig.hpp"
#include <string>
#include <filesystem>
#include <cassert>
#include "EnvironmentConfigSerializer.hpp"
#include "common/Fonts.hpp"
#include "core/PathConfig.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/EnvironmentConfigPopupRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentDataRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentDataChangedEvent.hpp"
#include "utils/FontUtils.hpp"
#include "utils/FileDialogUtils.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;


namespace EnvConfig 
{
    bool EnvironmentConfig::initialize(LightManager* manager) 
    {
        assert(manager && "LightManager가 비었습니다. 초기화 실패");
        if (!manager) { return false; }

        lightManager_ = manager;
        lightConfig_.initialize(lightManager_);

        auto envDataChangeID = AppEventSubscriber::get().subscribe<EnvironmentDataChangedEvent>([this](const EnvironmentDataChangedEvent& event)
            {
                this->onEnvironmentDataChanged(event);
            });
        appEventSubID_.push_back(envDataChangeID);

        return true;
    }

    void EnvironmentConfig::onEnvironmentDataChanged(const EnvironmentDataChangedEvent& event) 
    {
        switch (event.type) 
        {
        case EnvDataType::DataChanged: 
        {
            isDirty_ = true;
            lightConfig_.setFromManager();
            if (currentFileName_.empty() || currentFileName_.back() != '*')
            {
                currentFileName_ += "*";
            }
            break;
        }
        case EnvDataType::FileSaved:
        {
            isDirty_ = false;
            lightConfig_.setFromManager();

            currentFilePath_ = event.path;
            currentFileName_ = getFileName(currentFilePath_);
            break;
        }
        case EnvDataType::FileLoaded: 
        {
            isDirty_ = false;
            lightConfig_.setFromManager();

            if (event.path.empty())
            {
                currentFileName_ = "";
                currentFilePath_ = "";
            }
            else 
            {
                currentFilePath_ = event.path;
                currentFileName_ = getFileName(currentFilePath_);
            }
            break;
        }
        default:
            assert(0 && "정의되지 않은 EnvDataType이 ChangedEvent로 왔습니다.\n");
            break;
        }
    }

    void EnvironmentConfig::draw(bool isVisible)
    {
        if (!isVisible) { return; }

        bool isOpen = true;
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_Always);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

        if (ImGui::Begin("Environment Config", &isOpen, window_flags))
        {
            if (lightConfig_.draw()) 
            { 
                isDirty_ = true;
                AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvActionType::UIModify, "" });
            }

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
                if (currentFilePath_.empty()) 
                {
                    doSaveAs();
                }
                else 
                {
                    AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvActionType::Save, currentFilePath_ });
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Load", ImVec2(btnWidth2, btnHeight + 1.0f)))
            {
                if (isDirty_)
                {
                    pendingRequestType_ = EnvActionType::Load;
                    openConfirmDiscardTrigger_ = true;
                }
                else
                {
                    doLoad();
                }
            }

            ImGui::Dummy(ImVec2(0.0f, 3.0f));

            if (ImGui::Button("Save As", ImVec2(btnWidth3, btnHeight + 2.0f)))
            {
                doSaveAs();
            }

            ImGui::SameLine();
            if (ImGui::Button("Restore", ImVec2(btnWidth3, btnHeight + 2.0f)))
            {
                AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvActionType::Restore, currentFilePath_ });
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
            ImGui::Text("file: %s", toUTF8(currentFileName_).c_str());
            ImGui::PopStyleVar();
        }
        ImGui::End();

        //변경 상태 버려짐 팝업
        if (openConfirmDiscardTrigger_) 
        {
            ImGui::OpenPopup(getPopupTitleFromType(pendingRequestType_));
            openConfirmDiscardTrigger_ = false;
        }
    
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(272, 116));
        if (ImGui::BeginPopupModal(getPopupTitleFromType(pendingRequestType_), NULL, ImGuiWindowFlags_NoResize))
        {
            ImGui::PushFont(Fonts::Regular18);
            ImGui::Text(utf8("변경 사항이 버려집니다.\n 진행하시겠습니까?"));
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
            AppEventPublisher::get().publish(EnvironmentConfigPopupRequestedEvent{ false });
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

    void EnvironmentConfig::doSaveAs() 
    {
        string filePath = FileDialogUtils::SaveFile("JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0", "", "json", EnvPathConfig::userFilePath_.c_str());

        if (!filePath.empty())
        {
            filesystem::path absPath(filePath);
            AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvActionType::SaveAs, filePath });
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

    string EnvironmentConfig::getFileName(const string& path)
    {
        filesystem::path p(path);
        return p.filename().string();
    }
}
