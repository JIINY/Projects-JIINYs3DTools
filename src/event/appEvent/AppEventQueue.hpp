#pragma once
#include <queue>
#include <utility>
#include <mutex> //큐 접근에 대한 스레드 안정성 고려
#include <cassert>
#include "AppEventVariant.hpp"


class AppEventQueue 
{
public:
	static AppEventQueue& get();

	void push(const AppEventVariant& event) 
	{
		std::lock_guard<std::mutex> lock(mutex_); //멀티스레드 환경 대비: 락을 걸어 안전하게 Push
		queue_.push(event);
	}

	bool isEmpty() const 
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return queue_.empty();
	}

	AppEventVariant pop() 
	{
		assert(!queue_.empty() && "[에러] AppEvent큐가 비어있어 pop할 수 없습니다.");

		std::lock_guard<std::mutex> lock(mutex_);
		AppEventVariant event = std::move(queue_.front());
		queue_.pop();
		return event;
	}

private:
	std::queue<AppEventVariant> queue_;
	mutable std::mutex mutex_;

	AppEventQueue() = default;
	AppEventQueue(const AppEventQueue&) = delete;
	AppEventQueue& operator=(const AppEventQueue&) = delete;
};
