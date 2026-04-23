#pragma once
#include <queue>
#include <mutex>
#include <cassert>
#include "event/InputEventType.hpp"


class InputEventQueue {
public:
	static InputEventQueue& get()
	{
		static InputEventQueue instance;
		return instance;
	}

	void push(const InputEvent& event) 
	{ 
		std::lock_guard<std::mutex> lock(mutex_);
		queue_.push(event);
	}
	bool isEmpty() const 
	{ 
		std::lock_guard<std::mutex> lock(mutex_);
		return queue_.empty();
	}

	const InputEvent& front() const //읽기 전용
	{ 
		assert(!queue_.empty() && "InputEvent큐가 비어있어 front를 확인할 수 없습니다.");
		return queue_.front();
	}

	InputEvent pop() 
	{
		assert(!queue_.empty() && "InputEvent큐가 비어있어 pop할 수 없습니다.");

		std::lock_guard<std::mutex> lock(mutex_);
		InputEvent event = std::move(queue_.front());
		queue_.pop();
		return event;
	}

	void clear() 
	{
		while (!queue_.empty()) queue_.pop();
	}

private:
	std::queue<InputEvent> queue_;
	mutable std::mutex mutex_;

	InputEventQueue() = default;
	InputEventQueue(const InputEventQueue&) = delete;
	InputEventQueue& operator=(const InputEventQueue&) = delete;
};
