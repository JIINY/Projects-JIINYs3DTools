#include "InputHandler.hpp"
#include <imgui.h>
#include <unordered_map>
#include <variant>
#include <type_traits>
#include "InputEventQueue.hpp"
#include "event/InputEventType.hpp"

#include "common/DebugLog.hpp"
using namespace std;

void InputHandler::update(InputEventQueue& queue)
{
	for (auto it = keyStates_.begin(); it != keyStates_.end();)
	{
		int key = it->first;
		bool wasDown = it->second;
		bool nowDown = ImGui::IsKeyDown(static_cast<ImGuiKey>(key));

		if (wasDown && nowDown) 
		{
			keyDurations_[key]++;
			queue.push(KeyHoldEvent{ key, keyDurations_[key] });
			++it;
		}
		else if (wasDown && !nowDown) 
		{
			queue.push(KeyUpEvent { key });
			keyDurations_.erase(key);
			it = keyStates_.erase(it);
		}
		else 
		{
			++it;
		}
	}

	ImGuiIO& io = ImGui::GetIO();
	POINT currentPos = { (long)io.MousePos.x, (long)io.MousePos.y };
	if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f) //마우스 이동
	{
		queue.push(MouseMoveEvent{ io.MouseDelta.x, io.MouseDelta.y, currentPos });
	}

	for (int i = 0; i < 3; ++i) //마우스 버튼 클릭
	{
		if (ImGui::IsMouseClicked(i)) 
		{
			queue.push(MouseDownEvent{ i, currentPos });
		}
		if (ImGui::IsMouseReleased(i)) 
		{
			queue.push(MouseUpEvent{ i, currentPos });
		}
	}

	if (io.MouseWheel != 0.0f) 
	{
		queue.push(MouseWheelEvent{ io.MouseWheel, currentPos });
	}
}

void InputHandler::handleEvent(const InputEvent& event, InputEventQueue& queue) 
{
	visit([&](auto&& ev) {

		using T = decay_t<decltype(ev)>;
		if constexpr (is_same_v<T, KeyPressedEvent>) 
		{
			handleKeyPressed(ev, queue);
		}
		else //if constexpr
		{
			//확장용
		}
	}, event);
}

bool InputHandler::isKeyDown(int key) const 
{
	auto it = keyStates_.find(key);
	return it != keyStates_.end() && it->second;
}

void InputHandler::handleKeyPressed(const KeyPressedEvent& event, InputEventQueue& queue) 
{
	int key = event.keyCode_;

	auto it = keyStates_.find(key);
	if (it == keyStates_.end() || !it->second)
	{
		keyStates_[key] = true;
		keyDurations_[key] = 0;
		queue.push(KeyDownEvent{ key });
	}
}
