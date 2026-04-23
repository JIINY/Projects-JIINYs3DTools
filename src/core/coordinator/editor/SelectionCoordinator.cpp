#include "SelectionCoordinator.hpp"
#include <memory>
#include <vector>

#include "core/manager/scene/SceneObjectManager.hpp"
#include "core/manager/editor/ObjectSelectionManager.hpp"
#include "object/SceneObject.hpp"

#include "event/editorEvent/EditorEventPublisher.hpp"
#include "event/editorEvent/EditorEventSubscriber.hpp"
#include "event/editorEvent/selection/SelectionModeEvent.hpp"
#include "event/InputEventType.hpp"

#include "common/DebugLog.hpp"
using namespace std;


SelectionCoordinator::SelectionCoordinator() : objManager_(make_unique<ObjectSelectionManager>()) {}
SelectionCoordinator::~SelectionCoordinator() = default;

bool SelectionCoordinator::initialize(SelectionContext context) 
{
    objManager_ = make_unique<ObjectSelectionManager>();

    ObjectSelectionContext objContext;
    objContext.camCoord = context.camCoord;

    objContext.getCandidates = [sceneObj = context.sceneObjManager]() -> const vector<shared_ptr<SceneObject>>&
        {
            return sceneObj->getAllSceneObjects();
        };

    if (!objManager_->initialize(objContext)) { return false; }
    objManager_->onActivate();

    auto modeChangeID = EditorEventSubscriber::get().subscribe<SelectionModeChangedEvent>([this](const SelectionModeChangedEvent& event) 
        {
            this->onSelectionModeChanged(event.newMode);
        });
    editorEventSubID_.push_back(modeChangeID);

    auto selectionOneID = EditorEventSubscriber::get().subscribe<SelectionRequestedEvent>([this](const SelectionRequestedEvent& event) 
        {
            this->onSelectionRequested(event);
        });
    editorEventSubID_.push_back(selectionOneID);

    auto selectionDragID = EditorEventSubscriber::get().subscribe<SelectionDragRequestedEvent>([this](const SelectionDragRequestedEvent& event)
        {
            this->onSelectionDragRequested(event);
        });
    editorEventSubID_.push_back(selectionDragID);

    return true;
}

void SelectionCoordinator::update(float deltaTime) 
{
    switch (currentMode_) 
    {
    case SelectionMode::Object: objManager_->update(deltaTime); break;
    case SelectionMode::AnimationKey: break;
    default: break;
    }
}

void SelectionCoordinator::onSelectionModeChanged(SelectionMode mode)
{
    if (currentMode_ == mode) { return; }

    switch (currentMode_) 
    {
    case SelectionMode::Object: objManager_->onDeactivate(); break;
    case SelectionMode::AnimationKey: break;
    default: break;
    }
    
    currentMode_ = mode;
    selection_.clear();

    switch (currentMode_)
    {
    case SelectionMode::Object: objManager_->onActivate(); break;
    case SelectionMode::AnimationKey: break;
    default: break;
    }
}

void SelectionCoordinator::shutdown() 
{
    for (auto id : editorEventSubID_) 
    {
        EditorEventSubscriber::get().unsubscribe(id);
    }
    editorEventSubID_.clear();

    objManager_->shutdown();
}

void SelectionCoordinator::onSelectionRequested(const SelectionRequestedEvent& event) 
{
    if (!event.target) //빈 곳 클릭
    {
        if (!event.isMultiSelect && !event.isSubtract) 
        {
            selection_.clear();
        }
        return;
    }

    if (event.isMultiSelect) { selection_.toggle(event.target); }
    else if (event.isSubtract) { selection_.remove(event.target); }
    else 
    {
        selection_.select(event.target);
    }
}

void SelectionCoordinator::onSelectionDragRequested(const SelectionDragRequestedEvent& event) 
{
    vector<shared_ptr<Selection::Selectable>> targets(event.targets.begin(), event.targets.end());
    if (event.isAlt) 
    {
        selection_.removeRange(targets);
    }
    else if (event.isShift)
    {
        selection_.appendRange(targets);
    }
    else 
    {
        selection_.selectRange(targets);
    }
}

void SelectionCoordinator::clearSelection()
{
    selection_.clear();
}
