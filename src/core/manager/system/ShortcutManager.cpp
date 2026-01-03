#include "ShortcutManager.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/state/AppModeCycleRequestedEvent.hpp"
#include "imgui.h"


void ShortcutManager::processInputEvent(const InputEvent& event) 
{
	if (const KeyDownEvent* keyDownEvent = std::get_if<KeyDownEvent>(&event)) 
	{
		ImGuiIO& io = ImGui::GetIO();

		if (io.WantCaptureKeyboard) { return; }

		//단축키->이벤트 발행
		if (keyDownEvent->keyCode_ == ImGuiKey_Tab)
		{
			AppEventPublisher::get().publish(AppModeCycleRequestedEvent{});
		}
	}
}
