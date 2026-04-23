#include "ShortcutManager.hpp"
#include "command/CommandStack.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/state/AppModeCycleRequestedEvent.hpp"
#include "event/editorEvent/EditorEventPublisher.hpp"
#include "event/editorEvent/io/KeyboardEvent.hpp"
#include "event/editorEvent/transform/GizmoModeRequestedEvent.hpp"
#include "imgui.h"

bool ShortcutManager::initialize()
{
	auto keyDown = EditorEventSubscriber::get().subscribe<KeyDownEditorEvent>([this](const KeyDownEditorEvent& event)
		{
			this->onKeyDowned(event);
		});
	editorEventSubID_.push_back(keyDown);

	return true;
}

void ShortcutManager::onKeyDowned(const KeyDownEditorEvent& event) 
{
	if (!event.isCtrl && !event.isAlt && !event.isShift)
	{
		switch (event.keyCode)
		{
		case static_cast<int>(ImGuiKey_Tab):
		{
			AppEventPublisher::get().publish(AppModeCycleRequestedEvent{});
			break;
		}
		case static_cast<int>(ImGuiKey_W):
		{
			EditorEventPublisher::get().publish(GizmoModeRequestedEvent{ TransformMode::Translate });
			break;
		}
		case static_cast<int>(ImGuiKey_E):
		{
			EditorEventPublisher::get().publish(GizmoModeRequestedEvent{ TransformMode::Rotate });
			break;
		}
		case static_cast<int>(ImGuiKey_R):
		{
			EditorEventPublisher::get().publish(GizmoModeRequestedEvent{ TransformMode::Scale });
			break;
		}
		default:
			break;
		}
	}

	if (event.isCtrl && !event.isAlt && !event.isShift) //Ctrl only
	{
		switch (event.keyCode)
		{
		case static_cast<int>(ImGuiKey_Z):
		{
			CommandStack::get().undo();
			break;
		}
		case static_cast<int>(ImGuiKey_Y):
		{
			CommandStack::get().redo();
			break;
		}
		default:
			break;
		}
	}
}
