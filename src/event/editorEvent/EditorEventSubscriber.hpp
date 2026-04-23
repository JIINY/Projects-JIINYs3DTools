#pragma once
#include <functional>
#include <cstdint> //uint64_t 같은 타입 사용
#include <map>
#include <vector>
#include <typeindex>
#include <variant>
#include "EditorEventVariant.hpp"

using EditorEventSubscriptionID = uint64_t;
using EditorEventCallback = std::function<void(const EditorEventVariant&)>;


class EditorEventSubscriber
{
public:
	static EditorEventSubscriber& get();

	template <typename T>
	EditorEventSubscriptionID subscribe(std::function<void(const T&)> EditorEventCallback) //T타입의 이벤트를 받아 처리할 함수를 등록
	{
		auto eventHandler = [EditorEventCallback](const EditorEventVariant& variant)
			{
				if (const T* event = std::get_if<T>(&variant))
				{
					EditorEventCallback(*event);
				}
			};
		EditorEventSubscriptionID newID = nextID_++;
		callback_[std::type_index(typeid(T))].push_back({ newID, eventHandler });
		return newID;
	}

	void unsubscribe(EditorEventSubscriptionID id);

	void dispatch(const EditorEventVariant& event) //큐를 감시하다가 구독자에게 배달하는 집배원 역할
	{
		std::visit([this](const auto& event_in) //const auto&가 템플릿 역할(제네릭 람다) -> 헤더에 정의 필요
			{
				using EditorEventType = std::decay_t<decltype(event_in)>;

				auto it = callback_.find(std::type_index(typeid(EditorEventType)));
				if (it != callback_.end())
				{
					for (const auto& subscriberPair : it->second)
					{
						const EditorEventCallback& eventHandler = subscriberPair.second;
						eventHandler(event_in); //EditorEventCallback 호출
					}
				}
			}, event);
	}

private:
	std::map<std::type_index, std::vector<std::pair<EditorEventSubscriptionID, EditorEventCallback>>> callback_;
	EditorEventSubscriptionID nextID_ = 1;

	EditorEventSubscriber() = default;
	EditorEventSubscriber(const EditorEventSubscriber&) = delete;
	EditorEventSubscriber& operator=(const EditorEventSubscriber&) = delete;
};
