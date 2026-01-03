#pragma once
#include "event/InputEventType.hpp"


class ShortcutManager 
{
public:
	ShortcutManager() = default;
	~ShortcutManager() = default;

	void processInputEvent(const InputEvent& event);
};
