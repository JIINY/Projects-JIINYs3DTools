#include "ObjectSelectionManager.hpp"
#include <memory>
#include <Windows.h>
#include <variant>
#include <type_traits>
#include "core/coordinator/editor/CameraCoordinator.hpp"
#include "selection/ObjectSelectionController.hpp"

#include "event/editorEvent/EditorEventSubscriber.hpp"
#include "event/editorEvent/io/MouseEvent.hpp"
#include "object/SceneObject.hpp"
using namespace std;


ObjectSelectionManager::ObjectSelectionManager() : objController_(make_unique<ObjectSelectionController>()) {}
ObjectSelectionManager::~ObjectSelectionManager() = default;

bool ObjectSelectionManager::initialize(ObjectSelectionContext context)
{
    if (!objController_->initialize(context.camCoord)) { return false; }
    getCandidates_ = context.getCandidates;

    return true;
}

void ObjectSelectionManager::update(float deltaTime)
{
    //TO_DO: Hover시의 동작 등
}

void ObjectSelectionManager::shutdown()
{
    //초기화 역순
    objController_->shutdown();
}

void ObjectSelectionManager::onActivate()
{
    auto mouseDown = EditorEventSubscriber::get().subscribe<MouseDownEditorEvent>([this](const MouseDownEditorEvent& event)
        {
            this->onMouseDown(event);
        });
    editorEventSubID_.push_back(mouseDown);

    auto mouseUp = EditorEventSubscriber::get().subscribe<MouseUpEditorEvent>([this](const MouseUpEditorEvent& event)
        {
            this->onMouseUp(event);
        });
    editorEventSubID_.push_back(mouseUp);

    auto mouseMove = EditorEventSubscriber::get().subscribe<MouseMoveEditorEvent>([this](const MouseMoveEditorEvent& event)
        {
            this->onMouseMove(event);
        });
    editorEventSubID_.push_back(mouseMove);
}

void ObjectSelectionManager::onDeactivate() 
{
    for (auto id : editorEventSubID_)
    {
        EditorEventSubscriber::get().unsubscribe(id);
    }
    editorEventSubID_.clear();
    resetState();
}

void ObjectSelectionManager::resetState()
{
    isMouseDown_ = false;
    isDragging_ = false;
}

void ObjectSelectionManager::onMouseDown(const MouseDownEditorEvent& event) 
{
    if (event.button_ == 0) 
    {
        isMouseDown_ = true;
        isDragging_ = false;
        dragStartPos_ = event.pos_;
    }
}

void ObjectSelectionManager::onMouseUp(const MouseUpEditorEvent& event) 
{
    if (event.button_ != 0) { return; }
    if (!isMouseDown_) { return; }

    bool isShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    bool isAlt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

    const auto& candidates = getCandidates_();

    PickContext context;
    context.candidates = &candidates;
    context.dragStartPos = dragStartPos_;
    context.pos = event.pos_;
    context.isShift = isShift;
    context.isAlt = isAlt;
    if (isDragging_)
    {
        objController_->pickArea(context);
    }
    else 
    {
        objController_->pickObjectAt(context);
    }

    resetState();
}

void ObjectSelectionManager::onMouseMove(const MouseMoveEditorEvent& event) 
{
    if (isMouseDown_ && !isDragging_) 
    {
        float dx = static_cast<float>(event.pos_.x - dragStartPos_.x);
        float dy = static_cast<float>(event.pos_.y - dragStartPos_.y);

        if ((dx * dx + dy * dy) > (DRAG_THRESHOLD * DRAG_THRESHOLD)) 
        {
            isDragging_ = true;
        }
    }
}
