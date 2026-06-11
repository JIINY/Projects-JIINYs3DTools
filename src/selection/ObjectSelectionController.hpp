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

    Math::Ray calculateRay(int screenX, int screenY) const;
};
