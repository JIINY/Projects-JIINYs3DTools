#pragma once
#include <memory>
#include <functional>
#include <vector>
#include <Windows.h>
#include "event/editorEvent/EditorEventSubscriber.hpp"

class SceneObject;
class CameraCoordinator;
class ObjectSelectionController;

struct ObjectSelectionContext
{
    CameraCoordinator* camCoord = nullptr;
    std::function<const std::vector<std::shared_ptr<SceneObject>>& ()> getCandidates;
};


class ObjectSelectionManager
{
public:
    ObjectSelectionManager();
    ~ObjectSelectionManager();

    bool initialize(ObjectSelectionContext context);
    void update(float deltaTime);
    void shutdown();

    void onActivate();
    void onDeactivate();
    ObjectSelectionController* getObjController() { return objController_.get(); }
    void resetState();


private:
    std::vector<EditorEventSubscriptionID> editorEventSubID_;
    std::unique_ptr<ObjectSelectionController> objController_;
    std::function<const std::vector<std::shared_ptr<SceneObject>>& ()> getCandidates_;

    bool isMouseDown_ = false;
    bool isDragging_ = false;
    POINT dragStartPos_ = { 0, 0 };
    const float DRAG_THRESHOLD = 5.0f;

    void onMouseDown(const MouseDownEditorEvent& event);
    void onMouseUp(const MouseUpEditorEvent& event);
    void onMouseMove(const MouseMoveEditorEvent& event);
};
