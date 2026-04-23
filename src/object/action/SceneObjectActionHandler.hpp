#pragma once
#include "event/editorEvent/EditorEventSubscriber.hpp"
#include "event/editorEvent/io/KeyboardEvent.hpp"
#include "event/editorEvent/transform/GizmoDragEvent.hpp"
#include "command/CommandStack.hpp"
#include "command/passiveObject/CmdDeletePrimitive.hpp"

class SelectionCoordinator;
class SceneObjectManager;

struct SceneObjectActionContext
{
    SceneObjectManager* sceneObjManager = nullptr;
    SelectionCoordinator* selectionCoord = nullptr;
};

struct DragState
{
    std::shared_ptr<SceneObject> obj;
    Math::Vec3 beginPos = {};
    Math::Vec3 beginRot = {};
    Math::Vec3 beginScale = {};
};


class SceneObjectActionHandler
{
public:
    bool initialize(SceneObjectActionContext context);


private:
    std::vector<EditorEventSubscriptionID> editorEventSubID_;
    std::vector<DragState> dragStates_;
    SceneObjectManager* sceneObjManager_ = nullptr;
    SelectionCoordinator* selectionCoord_ = nullptr;

    void onKeyDowned(const KeyDownEditorEvent& event);
    void onGizmoDragged(const GizmoDragEvent& event);
    void deleteSelected();
};
