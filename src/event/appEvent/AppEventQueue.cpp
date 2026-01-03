#include "AppEventQueue.hpp"

AppEventQueue& AppEventQueue::get() 
{
	static AppEventQueue instance;
	return instance;
}
