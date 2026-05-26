#pragma once
#include <memory>
#include <vector>
#include "event/InputEventType.hpp"
#include "event/editorEvent/EditorEventSubscriber.hpp"
#include "selection/SelectionList.hpp"

struct SelectionModeChangedEvent;
struct SelectionRequestedEvent;
struct SelectionDragRequestedEvent;
struct SelectionStateRequestedEvent;
class CameraCoordinator;
class SceneObjectManager;
class ObjectSelectionManager;

struct SelectionContext 
{
    CameraCoordinator* camCoord = nullptr;
    SceneObjectManager* sceneObjManager = nullptr;
};


class SelectionCoordinator 
{
public:
    SelectionCoordinator();
    ~SelectionCoordinator();

    bool initialize(SelectionContext context);
    void update(float deltaTime);
    void shutdown();

    void clearSelection();

    ObjectSelectionManager* getObjectSelectionManager() const { return objManager_.get(); }
    const Selection::SelectionList& getSelectionList() const { return selection_; }

private:
    std::vector<EditorEventSubscriptionID> editorEventSubID_;
    std::unique_ptr<ObjectSelectionManager> objManager_;
    Selection::SelectionList selection_;
    SelectionMode currentMode_ = SelectionMode::Object;

    void onSelectionModeChanged(SelectionMode mode);
    void onSelectionRequested(const SelectionRequestedEvent& event);
    void onSelectionDragRequested(const SelectionDragRequestedEvent& event);
    void onSelectionStateRequested();
};
