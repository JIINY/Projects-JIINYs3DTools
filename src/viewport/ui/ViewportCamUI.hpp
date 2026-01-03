#pragma once
#include <string>
#include <map>
#include "common/Mode.hpp"

class ViewportCamUI {
public:
	ViewportCamUI();
	ViewportCamUI(int x, int y, float a) : posX_(x), posY_(y), bgAlpha_(a) {}

	void setString(CameraMode camMode, ViewMode viewMode);
	
	void initialize(CameraMode camMode, ViewMode viewMode);
	void draw();


private:
	std::string label_ = "Arm Camera";
	std::map<CameraMode, std::string> camModeLabel_;
	std::map<ViewMode, std::string> viewModeLabel_;

	int posX_ = 8;
	int posY_ = 25;
	float bgAlpha_ = 0.0f;
};
