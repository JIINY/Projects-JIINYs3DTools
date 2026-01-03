#pragma once
#include "common/Mode.hpp"


struct CameraModeChangedEvent 
{
	CameraMode newCam_;
	ViewMode newView_;

	CameraModeChangedEvent(CameraMode cam, ViewMode view) : newCam_(cam), newView_(view) {}
};
