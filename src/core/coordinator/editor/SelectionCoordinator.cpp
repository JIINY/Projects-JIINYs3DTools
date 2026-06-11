#include "SelectionCoordinator.hpp"
#include <memory>
#include <vector>

#include "core/manager/scene/SceneObjectManager.hpp"
#include "core/manager/editor/ObjectSelectionManager.hpp"
#include "core/manager/editor/ToolObjectManager.hpp"
#include "object/SceneObject.hpp"

#include "command/CommandStack.hpp"
#include "command/selection/CmdChangeSelection.hpp"
#include "event/editorEvent/EditorEventPublisher.hpp"
#include "event/editorEvent/EditorEventSubscriber.hpp"
#include "event/editorEvent/selection/SelectionModeEvent.hpp"
#include "event/editorEvent/selection/SelectionChangeEvent.hpp"
#include "event/editorEvent/selection/SelectionStateEvent.hpp"
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
    objContext.gizmoCon = context.toolObjManager->getGizmoController();

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

    auto selectionStateID = EditorEventSubscriber::get().subscribe<SelectionStateRequestedEvent>([this](const SelectionStateRequestedEvent& event)
        {
            this->onSelectionStateRequested();
        });
    editorEventSubID_.push_back(selectionStateID);

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

void SelectionCoordinator::onSelectionStateRequested()
{
    const auto& selection = selection_.getAll();

    SelectionStateProvidedEvent event;
    event.selectionCount = static_cast<int>(selection_.count());
    event.currentSelection = selection;

    auto last = selection_.getLastSelected();
    event.currentType = last ? last->getSelectableType() : Selection::SelectableType::Count;

    EditorEventPublisher::get().publish(event);
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
    auto before = selection_.getAll();
    vector<shared_ptr<Selection::Selectable>> after;

    if (!event.target) //빈 곳 클릭
    {
        if (!event.isMultiSelect && !event.isSubtract) 
        {
            CommandStack::get().execute(make_shared<CmdChangeSelection>(&selection_, before, vector<shared_ptr<Selection::Selectable>>{}));
        }
        return;
    }

    if (event.isMultiSelect)
    {
        after = before;
        auto it = find(after.begin(), after.end(), event.target);

        if (it != after.end()) { after.erase(it); }
        else { after.push_back(event.target); }
    }
    else if (event.isSubtract)
    {
        after = before;
        auto it = find(after.begin(), after.end(), event.target);

        if (it != after.end()) { after.erase(it); }
    }
    else 
    {
        if (before.size() == 1 && before[0].get() == event.target.get())
        {
            after = {};
        }
        else
        {
            after = { event.target };
        }
    }

    CommandStack::get().execute(make_shared<CmdChangeSelection>(&selection_, move(before), move(after)));
}

void SelectionCoordinator::onSelectionDragRequested(const SelectionDragRequestedEvent& event) 
{
    auto before = selection_.getAll();
    vector<shared_ptr<Selection::Selectable>> targets(event.targets.begin(), event.targets.end());
    vector<shared_ptr<Selection::Selectable>> after;

    if (event.isAlt) 
    {
        after = before;
        for (const auto& item : targets)
        {
            auto it = find(after.begin(), after.end(), item);
            if (it != after.end()) { after.erase(it); }
        }
    }
    else if (event.isShift)
    {
        after = before;
        for (const auto& item : targets)
        {
            if (find(after.begin(), after.end(), item) == after.end())
            {
                after.push_back(item);
            }
        }
    }
    else 
    {
        after = targets;
    }
    CommandStack::get().execute(make_shared<CmdChangeSelection>(&selection_, move(before), move(after)));
}

void SelectionCoordinator::clearSelection()
{
    auto before = selection_.getAll();
    if (before.empty()) { return; }
    CommandStack::get().execute(make_shared<CmdChangeSelection>(&selection_, move(before), vector<shared_ptr<Selection::Selectable>>{}));
}
