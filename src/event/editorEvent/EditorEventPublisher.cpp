#include "EditorEventPublisher.hpp"
#include "EditorEventQueue.hpp"


EditorEventPublisher& EditorEventPublisher::get()
{
	static EditorEventPublisher instance;
	return instance;
}

void EditorEventPublisher::publish(const EditorEventVariant& event)
{
	EditorEventQueue::get().push(event);
}
