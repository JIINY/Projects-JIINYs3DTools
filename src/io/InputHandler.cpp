#include "InputHandler.hpp"
#include <unordered_map>
#include <variant>
#include <type_traits>
#include "InputEventQueue.hpp"
#include "event/InputEventType.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;

void InputHandler::update()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard) { return; }

	for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; ++i)
	{
		ImGuiKey key = static_cast<ImGuiKey>(i);

		//IsKeyPressed는 이번 프레임에 처음 눌렸는가를 확인
		if (ImGui::IsKeyPressed(key, false))
		{
			auto it = keyStates_.find(i);
			if (it == keyStates_.end() || !it->second)
			{
				keyStates_[i] = true;
				keyDurations_[i] = 0;
				InputEventQueue::get().push(KeyDownEvent{ i, io.KeyCtrl, io.KeyAlt, io.KeyShift });
			}
		}
	}

	for (auto it = keyStates_.begin(); it != keyStates_.end();)
	{
		int key = it->first;
		bool wasDown = it->second;
		bool nowDown = ImGui::IsKeyDown(static_cast<ImGuiKey>(key));

		if (wasDown && nowDown) 
		{
			keyDurations_[key]++;
			InputEventQueue::get().push(KeyHoldEvent{ key, keyDurations_[key], io.KeyCtrl, io.KeyAlt, io.KeyShift });
			++it;
		}
		else if (wasDown && !nowDown) 
		{
			InputEventQueue::get().push(KeyUpEvent { key, io.KeyCtrl, io.KeyAlt, io.KeyShift });
			keyDurations_.erase(key);
			it = keyStates_.erase(it);
		}
		else 
		{
			++it;
		}
	}

	if (io.WantCaptureMouse) { return; }

	POINT currentPos = { (long)io.MousePos.x, (long)io.MousePos.y };
	if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f) //마우스 이동
	{
		InputEventQueue::get().push(MouseMoveEvent{ io.MouseDelta.x, io.MouseDelta.y, currentPos, io.KeyCtrl, io.KeyAlt, io.KeyShift });
	}

	for (int i = 0; i < 3; ++i) //마우스 버튼 클릭
	{
		if (ImGui::IsMouseClicked(i)) 
		{
			InputEventQueue::get().push(MouseDownEvent{ i, currentPos, io.KeyCtrl, io.KeyAlt, io.KeyShift });
		}
		else if (ImGui::IsMouseDown(i))
		{
			InputEventQueue::get().push(MouseHoldEvent{ i, io.MouseDelta.x, io.MouseDelta.y, currentPos, io.KeyCtrl, io.KeyAlt, io.KeyShift });
		}
		if (ImGui::IsMouseReleased(i))
		{
			InputEventQueue::get().push(MouseUpEvent{ i, currentPos, io.KeyCtrl, io.KeyAlt, io.KeyShift });
		}
	}

	if (io.MouseWheel != 0.0f) 
	{
		InputEventQueue::get().push(MouseWheelEvent{ io.MouseWheel, currentPos, io.KeyCtrl, io.KeyAlt, io.KeyShift });
	}
}

bool InputHandler::isKeyDown(int key) const 
{
	auto it = keyStates_.find(key);
	return it != keyStates_.end() && it->second;
}
