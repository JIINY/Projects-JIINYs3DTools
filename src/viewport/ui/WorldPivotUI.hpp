#pragma once
#include "core/coordinator/editor/CameraCoordinator.hpp"
#include "imgui.h"

class CameraController;


class WorldPivotUI 
{
public:
    void draw(CameraController* activeCamController, const ImVec2& viewportPos, const ImVec2& viewportSize);


private:
    float axisSize_ = 90.0f;
    float axisLength_ = 25.0f;
    float padding_ = 100.0f;
    float thickness_ = 1.0f;
};
