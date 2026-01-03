#include "AppUI.hpp"
#include "common/Mode.hpp"
#include "common/ElementID.hpp"
#include "core/manager/system/AppUIManager.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/state/AppModeCycleRequestedEvent.hpp"

#include "imgui.h"
#include "imgui_internal.h"


AppUI::AppUI() : appUIManager_(nullptr) {}

void AppUI::initialize(AppUIManager* manager) 
{
	appUIManager_ = manager;
}

void AppUI::draw() 
{
	drawModeButton();
}


void AppUI::drawModeButton()
{
	//모드 버튼
	const char* mode = AppModeName.at(static_cast<size_t>(appUIManager_->getMode()));
	
	ImVec2 windowSize = ImGui::GetIO().DisplaySize;
	ImVec2 buttonSize = ImVec2(120, 40);
	ImVec2 centerPos = ImVec2(
		(windowSize.x - buttonSize.x) * 0.5f,
		(buttonSize.y * 0.5f) + 3
	);

	ImGui::SetNextWindowPos(centerPos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(buttonSize, ImGuiCond_Always);
	
	if (ImGui::Begin("ModeButton", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse)) 
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); //패딩 제거
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(30, 30, 30, 255)); //테두리 컬러

		if (ImGui::InvisibleButton(mode, buttonSize)) //캔버스 자체가 버튼으로 동작하도록 InvisibleButton
		{
			AppEventPublisher::get().publish(AppModeCycleRequestedEvent{});
		}

		ImVec2 pMin = ImGui::GetWindowPos();
		ImVec2 pMax = ImVec2(pMin.x + buttonSize.x, pMin.y + buttonSize.y);

		//마우스 호버일 때 색상 변경
		if (ImGui::IsItemHovered())
		{
			ImGui::GetWindowDrawList()->AddRectFilled(pMin, pMax, IM_COL32(120, 180, 245, 255));
		}
		else
		{
			ImGui::GetWindowDrawList()->AddRectFilled(pMin, pMax, IM_COL32(80, 80, 80, 255));
		}

		//모드버튼 글씨 중앙에 출력
		ImVec2 textSize = ImGui::CalcTextSize(mode);
		ImVec2 textPos = ImVec2(
			(pMin.x + pMax.x - textSize.x) * 0.5f,
			(pMin.y + pMax.y - textSize.y) * 0.5f
		);

		ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), mode);

		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar();
		ImGui::End();
	}
}
