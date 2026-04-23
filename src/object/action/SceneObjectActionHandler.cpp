#include "SceneObjectActionHandler.hpp"
#include "viewport/ui/GizmoController.hpp"
#include "event/InputEventType.hpp"
#include "object/SceneObject.hpp"
#include "core/coordinator/editor/SelectionCoordinator.hpp"
#include "core/coordinator/editor/CameraCoordinator.hpp"
#include "core/manager/scene/SceneObjectManager.hpp"

#include "event/editorEvent/EditorEventSubscriber.hpp"
#include "event/editorEvent/io/KeyboardEvent.hpp"
#include "event/editorEvent/transform/GizmoDragEvent.hpp"

#include "command/CommandStack.hpp"
#include "command/passiveObject/CmdDeletePrimitive.hpp"
#include "command/sceneObject/CmdMoveSceneObject.hpp"
#include "command/sceneObject/CmdRotateSceneObject.hpp"
#include "command/sceneObject/CmdScaleSceneObject.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;
using namespace Math;


bool SceneObjectActionHandler::initialize(SceneObjectActionContext context)
{
    sceneObjManager_ = context.sceneObjManager;
    selectionCoord_ = context.selectionCoord;

    auto keyDown = EditorEventSubscriber::get().subscribe<KeyDownEditorEvent>([this](const KeyDownEditorEvent& event)
        {
            this->onKeyDowned(event);
        });
    editorEventSubID_.push_back(keyDown);

    auto gizmoDrag = EditorEventSubscriber::get().subscribe<GizmoDragEvent>([this](const GizmoDragEvent& event)
        {
            this->onGizmoDragged(event);
        });
    editorEventSubID_.push_back(gizmoDrag);

    return true;
}

void SceneObjectActionHandler::onKeyDowned(const KeyDownEditorEvent& event)
{
    if (!event.isCtrl && !event.isAlt && !event.isShift)
    {
        if (event.keyCode == static_cast<int>(ImGuiKey_Delete))
        {
            deleteSelected();
        }
    }
}

void SceneObjectActionHandler::onGizmoDragged(const GizmoDragEvent& event)
{
    if (selectionCoord_->getSelectionList().isEmpty()) { return; }
    const auto& selection = selectionCoord_->getSelectionList().getAll();

    if (event.state == GizmoDragState::Begin)
    {
        dragStates_.clear();
        for (auto& o : selection)
        {
            auto sceneObj = dynamic_pointer_cast<SceneObject>(o);
            if (!sceneObj) { continue; }

            DragState ds;
            ds.obj = sceneObj;
            ds.beginPos = sceneObj->getPosition();
            ds.beginRot = sceneObj->getRotation();
            ds.beginScale = sceneObj->getScale();
            dragStates_.push_back(ds);
        }
        return;
    }

    if (event.state == GizmoDragState::Update)
    {
        for (auto& ds : dragStates_)
        {
            if (!ds.obj) { continue; }

            Vec3 deltaVec = {};
            switch (event.axis)
            {
            case Axis::X: deltaVec.x = event.delta; break;
            case Axis::Y: deltaVec.y = event.delta; break;
            case Axis::Z: deltaVec.z = event.delta; break;
            default: return;
            }

            switch (event.mode)
            {
            case TransformMode::Translate:
            {
                Math::Vec3 pos = ds.obj->getPosition();
                pos.x += deltaVec.x;
                pos.y += deltaVec.y;
                pos.z += deltaVec.z;
                ds.obj->setPosition(pos);
                break;
            }
            case TransformMode::Rotate:
            {
                Math::Vec3 rot = ds.obj->getRotation();
                rot.x += deltaVec.x;
                rot.y += deltaVec.y;
                rot.z += deltaVec.z;
                ds.obj->setRotation(rot);
                break;
            }
            case TransformMode::Scale:
            {
                Math::Vec3 scale = ds.obj->getScale();
                scale.x += deltaVec.x;
                scale.y += deltaVec.y;
                scale.z += deltaVec.z;
                ds.obj->setScale(scale);
                break;
            }
            default: break;
            }
        }
        return;
    }

    if (event.state == GizmoDragState::End)
    {
        for (auto& ds : dragStates_)
        {
            if (!ds.obj) { continue; }

            switch (event.mode)
            {
            case TransformMode::Translate: CommandStack::get().execute(make_shared<CmdMoveSceneObject>(ds.obj, ds.beginPos, ds.obj->getPosition())); break;
            case TransformMode::Rotate: CommandStack::get().execute(make_shared<CmdRotateSceneObject>(ds.obj, ds.beginRot, ds.obj->getRotation())); break;
            case TransformMode::Scale: CommandStack::get().execute(make_shared<CmdScaleSceneObject>(ds.obj, ds.beginScale, ds.obj->getScale())); break;
            default: break;
            }
        }

        dragStates_.clear();
        return;
    }
}

void SceneObjectActionHandler::deleteSelected()
{
    if (!sceneObjManager_ || !selectionCoord_) { return; }

    const auto& selected = selectionCoord_->getSelectionList().getAll();
    if (selected.empty()) { return; }

    vector<shared_ptr<SceneObject>> passiveTargets;
    for (const auto& sel : selected)
    {
        if (auto obj = dynamic_pointer_cast<SceneObject>(sel))
        {
            switch (obj->getObjectType())
            {
            case SceneObjectType::BaseObject:
            {
                passiveTargets.push_back(obj);
                break;
            }
            default:
                break;
            }
        }
    }

    if (!passiveTargets.empty())
    {
        CommandStack::get().execute(
            make_shared<CmdDeletePrimitive>(sceneObjManager_->getPassiveCoordinator(), passiveTargets)
        );
    }

    selectionCoord_->clearSelection();
}
