#include "UIEventSubscriber.hpp"
#include <any>


struct Subscriber //구조적 프로그래밍의 파일 스코프
{
	std::any instance;
};


UIEventSubscriber& UIEventSubscriber::get()
{
	static UIEventSubscriber instance;
	return instance;
}

void UIEventSubscriber::unsubscribe(UIEventSubscriptionID id)
{
	for (auto& [type, subscribers] : callback_)
	{
		std::erase_if(subscribers, [id](const auto& pair)
			{
				return pair.first == id;
			});
	}
}
