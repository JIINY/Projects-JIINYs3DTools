#pragma once
#include <functional>
#include <cstdint> //uint64_t 같은 타입 사용
#include <map>
#include <vector>
#include <typeindex>
#include <variant>
#include "AppEventVariant.hpp"

using AppEventSubscriptionID = uint64_t;
using AppEventCallback = std::function<void(const AppEventVariant&)>;


class AppEventSubscriber 
{
public:
	static AppEventSubscriber& get();

	template <typename T>
	AppEventSubscriptionID subscribe(std::function<void(const T&)> appEventCallback) //T타입의 이벤트를 받아 처리할 함수를 등록
	{
		auto eventHandler = [appEventCallback](const AppEventVariant& variant)
			{
				if (const T* event = std::get_if<T>(&variant))
				{
					appEventCallback(*event);
				}
			};
		AppEventSubscriptionID newID = nextID_++;
		callback_[std::type_index(typeid(T))].push_back({newID, eventHandler});
		return newID;
	}

	void unsubscribe(AppEventSubscriptionID id);

	void dispatch(const AppEventVariant& event) //큐를 감시하다가 구독자에게 배달하는 집배원 역할
	{
		std::visit([this](const auto& event_in) //const auto&가 템플릿 역할(제네릭 람다) -> 헤더에 정의 필요
		{
			using AppEventType = std::decay_t<decltype(event_in)>;

			auto it = callback_.find(std::type_index(typeid(AppEventType)));
			if (it != callback_.end())
			{
				for (const auto& subscriberPair : it->second)
				{
					const AppEventCallback& eventHandler = subscriberPair.second;
					eventHandler(event_in); //AppEventCallback 호출
				}
			}
		}, event);
	}

private:
	std::map<std::type_index, std::vector<std::pair<AppEventSubscriptionID, AppEventCallback>>> callback_;
	AppEventSubscriptionID nextID_ = 1;

	AppEventSubscriber() = default;
	AppEventSubscriber(const AppEventSubscriber&) = delete;
	AppEventSubscriber& operator=(const AppEventSubscriber&) = delete;
};
