#pragma once

class ViewportCameraManager;


class CameraInfoUI
{
public:
	explicit CameraInfoUI() = default;

	void initialize(ViewportCameraManager* manager);
	void draw(bool isVisible);

private:
	ViewportCameraManager* manager_ = nullptr;
};
