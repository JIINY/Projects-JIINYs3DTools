#pragma once
#include <functional>
#include <cstdint> //uint64_t 같은 타입 사용
#include <map>
#include <vector>
#include <typeindex>
#include <variant>
#include "UIEventVariant.hpp"

using UIEventSubscriptionID = uint64_t;
using UIEventCallback = std::function<void(const UIEventVariant&)>;


class UIEventSubscriber
{
public:
	static UIEventSubscriber& get();

	template <typename T>
	UIEventSubscriptionID subscribe(std::function<void(const T&)> UIEventCallback)
	{
		auto eventHandler = [UIEventCallback](const UIEventVariant& variant)
			{
				if (const T* event = std::get_if<T>(&variant))
				{
					UIEventCallback(*event);
				}
			};
		UIEventSubscriptionID newID = nextID_++;
		callback_[std::type_index(typeid(T))].push_back({ newID, eventHandler });
		return newID;
	}

	void unsubscribe(UIEventSubscriptionID id);

	void dispatch(const UIEventVariant& event)
	{
		std::visit([this](const auto& event_in)
			{
				using UIEventType = std::decay_t<decltype(event_in)>;

				auto it = callback_.find(std::type_index(typeid(UIEventType)));
				if (it != callback_.end())
				{
					for (const auto& subscriberPair : it->second)
					{
						const UIEventCallback& eventHandler = subscriberPair.second;
						eventHandler(event_in);
					}
				}
			}, event);
	}

private:
	std::map<std::type_index, std::vector<std::pair<UIEventSubscriptionID, UIEventCallback>>> callback_;
	UIEventSubscriptionID nextID_ = 1;

	UIEventSubscriber() = default;
	UIEventSubscriber(const UIEventSubscriber&) = delete;
	UIEventSubscriber& operator=(const UIEventSubscriber&) = delete;
};
