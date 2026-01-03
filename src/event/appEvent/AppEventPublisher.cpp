#include "AppEventPublisher.hpp"
#include "AppEventQueue.hpp"


AppEventPublisher& AppEventPublisher::get() 
{
	static AppEventPublisher instance;
	return instance;
}

void AppEventPublisher::publish(const AppEventVariant& event) 
{
	AppEventQueue::get().push(event);
}
