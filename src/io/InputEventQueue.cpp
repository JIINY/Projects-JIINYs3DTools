#include "InputEventQueue.hpp"

InputEventQueue& InputEventQueue::get()
{
	static InputEventQueue instance;
	return instance;
}
