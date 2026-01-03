#include "AppEventSubscriber.hpp"
#include <any>


struct Subscriber //구독자 정보를 저장하기 위한 내부 구조체
{
	std::any instance;
};


AppEventSubscriber& AppEventSubscriber::get() 
{
	static AppEventSubscriber instance;
	return instance;
}

void AppEventSubscriber::unsubscribe(AppEventSubscriptionID id) 
{
	for (auto& [type, subscribers] : callback_)
	{
		std::erase_if(subscribers, [id](const auto& pair) 
			{
				return pair.first == id;
			});
	}
}
