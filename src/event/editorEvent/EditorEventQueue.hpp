#pragma once
#include <queue>
#include <utility>
#include <mutex> //큐 접근에 대한 스레드 안정성 고려
#include <cassert>
#include "EditorEventVariant.hpp"


class EditorEventQueue
{
public:
	static EditorEventQueue& get()
	{
		static EditorEventQueue instance;
		return instance;
	}

	void push(const EditorEventVariant& event)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		queue_.push(event);
	}

	bool isEmpty() const
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return queue_.empty();
	}

	EditorEventVariant pop()
	{
		assert(!queue_.empty() && "EditorEvent큐가 비어있어 pop할 수 없습니다.");

		std::lock_guard<std::mutex> lock(mutex_);
		EditorEventVariant event = std::move(queue_.front());
		queue_.pop();
		return event;
	}

private:
	std::queue<EditorEventVariant> queue_;
	mutable std::mutex mutex_;

	EditorEventQueue() = default;
	EditorEventQueue(const EditorEventQueue&) = delete;
	EditorEventQueue& operator=(const EditorEventQueue&) = delete;
};
