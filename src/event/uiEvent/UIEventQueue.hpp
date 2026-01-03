#pragma once
#include <queue>
#include <utility>
#include <mutex> //큐 접근에 대한 스레드 안정성 고려
#include <cassert>
#include "UIEventVariant.hpp"


class UIEventQueue
{
public:
	static UIEventQueue& get();

	void push(const UIEventVariant& event)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		queue_.push(event);
	}

	bool isEmpty() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return queue_.empty();
	}

	UIEventVariant pop()
	{
		assert(!queue_.empty() && "[에러] UIEvent큐가 비어있어 pop할 수 없습니다.");

		std::lock_guard<std::mutex> lock(mutex_);
		UIEventVariant event = std::move(queue_.front());
		queue_.pop();
		return event;
	}

private:
	std::queue<UIEventVariant> queue_;
	mutable std::mutex mutex_;

	UIEventQueue() = default;
	UIEventQueue(const UIEventQueue&) = delete;
	UIEventQueue& operator=(const UIEventQueue&) = delete;
};
