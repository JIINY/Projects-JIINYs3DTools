#include "ViewportCamUI.hpp"
#include <string>
#include <map>
#include "common/Fonts.hpp"
#include "imgui.h"


ViewportCamUI::ViewportCamUI() 
{
	camModeLabel_[CameraMode::FreeArm] = "Arm Camera";
	camModeLabel_[CameraMode::Target] = "Target Camera";
	camModeLabel_[CameraMode::FreeFly] = "Free Camera";
	viewModeLabel_[ViewMode::TopView] = "Top View";

	viewModeLabel_[ViewMode::FrontView] = "Front View";
	viewModeLabel_[ViewMode::LeftView] = "Left View";
	viewModeLabel_[ViewMode::RightView] = "Right View";
	viewModeLabel_[ViewMode::BackView] = "Back View";
	viewModeLabel_[ViewMode::BottomView] = "Bottom View";
}


void ViewportCamUI::setString(CameraMode camMode, ViewMode viewMode) 
{
	if (camMode != CameraMode::FixedAngle) 
	{
		auto it = camModeLabel_.find(camMode);
		if (it != camModeLabel_.end()) 
		{
			label_ = it->second;
		}
		else 
		{
			label_ = "? Camera";
		}
	}
	else 
	{
		auto it = viewModeLabel_.find(viewMode);
		if (it != viewModeLabel_.end()) 
		{
			label_ = it->second;
		}
		else
		{
			label_ = "? View";
		}
	}
}

void ViewportCamUI::initialize(CameraMode camMode, ViewMode viewMode) 
{
	setString(camMode, viewMode);
}

void ViewportCamUI::draw() 
{
	ImGui::SetNextWindowPos(ImVec2(posX_, posY_), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(bgAlpha_);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(180, 180, 180, 255));

	ImGuiWindowFlags flags = 
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoNav |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoInputs;

	ImGui::Begin("CameraViewLabel", nullptr, flags);

	ImGui::PushFont(Fonts::Regular17);
	ImGui::TextUnformatted(label_.c_str());
	ImGui::PopFont();

	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(1);
	ImGui::End();
}
