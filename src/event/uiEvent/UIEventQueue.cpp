#include "UIEventQueue.hpp"

UIEventQueue& UIEventQueue::get()
{
	static UIEventQueue instance;
	return instance;
}
