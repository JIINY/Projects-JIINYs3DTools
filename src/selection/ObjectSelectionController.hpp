#pragma once
#include <vector>
#include <memory>
#define NOMINMAX
#include <Windows.h>
#include "common/Math.hpp"

class SceneObject;
class CameraCoordinator;
class ViewportCameraManager;


struct PickContext 
{
    POINT pos = { 0, 0 };
    POINT dragStartPos = { 0, 0 };
    const std::vector<std::shared_ptr<SceneObject>>* candidates = nullptr;
    bool isShift = false;
    bool isAlt = false;
    float viewportWidth = 0.0f; //Window크기 그대로 사이즈를 받으면 안됨. mainMenuBar크기를 빼서 실제 Viewport크기로 계산 필요
    float viewportHeight = 0.0f;
};

class ObjectSelectionController
{
public:
    bool initialize(CameraCoordinator* camCoord);
    void shutdown() {}

    void pickObjectAt(PickContext context);
    void pickArea(PickContext context);


private:
    ViewportCameraManager* viewportManager_ = nullptr;

    Math::Ray calculateRay(const PickContext& context) const;
};
