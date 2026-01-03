#include "UIEventPublisher.hpp"
#include "UIEventQueue.hpp"


UIEventPublisher& UIEventPublisher::get()
{
	static UIEventPublisher instance;
	return instance;
}

void UIEventPublisher::publish(const UIEventVariant& event)
{
	UIEventQueue::get().push(event);
}
