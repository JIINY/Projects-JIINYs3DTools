#pragma once
#include <unordered_map>
#include "event/InputEventType.hpp"

class InputEventQueue;

class InputHandler {
public:
	void update();

private:
	std::unordered_map<int, bool> keyStates_;
	std::unordered_map<int, int> keyDurations_; //holdCount
	
	//ImGui 내부 키 상태를 얻어내기 위함
	bool isKeyDown(int key) const;
};
