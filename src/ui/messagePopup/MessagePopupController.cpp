#include "MessagePopupController.hpp"
#include "utils/FontUtils.hpp"
#include "imgui.h"

#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/message/MessagePopupRequestedEvent.hpp"
using namespace std;


bool MessagePopupController::initialize()
{
    auto msgPopupID = UIEventSubscriber::get().subscribe<MessagePopupRequestedEvent>([this](const MessagePopupRequestedEvent& event)
        {
            onMessagePopupRequested(event);
        });
    uiEventSubID_.push_back(msgPopupID);

    return true;
}

void MessagePopupController::onMessagePopupRequested(const MessagePopupRequestedEvent& event)
{
    title_ = event.title;
    pendingMsg_ = event.message;
    openTrigger_ = true;
}

void MessagePopupController::draw()
{
    if (openTrigger_)
    {
        ImGui::OpenPopup(title_.c_str());
        openTrigger_ = false;
    }

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSizeConstraints(ImVec2(240, 0), ImVec2(240, 1200));
    if (ImGui::BeginPopupModal(title_.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("%s", toUTF8(pendingMsg_).c_str());
        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        float buttonWidth = 100.0f;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
        if (ImGui::Button(utf8("확인"), ImVec2(buttonWidth, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}
