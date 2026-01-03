#pragma once
#include <vector>
#include <algorithm>
#include <mutex>
#include <cassert>
#include "RenderCommand.hpp"


class RenderCommandQueue {
public:
	static RenderCommandQueue& get();

	void addCommand(const RenderCommand& cmd) 
	{ 
		//lock_guard<std::mutex> lock(mutex_);
		queue_.push_back(cmd); 
	}

	void sort() 
	{
		//lock_guard<std::mutex> lock(mutex_);
		std::sort(queue_.begin(), queue_.end(),
			[](const RenderCommand& a, const RenderCommand& b)
			{
				if (a.layer_ != b.layer_) return a.layer_ < b.layer_;
				return a.sortKey_ < b.sortKey_;
			});
	}

	const std::vector<RenderCommand>& getQueue() const { return queue_; }
	void clear() { queue_.clear(); }
	bool isEmpty() const { return queue_.empty(); }
	std::size_t getSize() const { return queue_.size(); }

private:
	std::vector<RenderCommand> queue_;
	//std::mutex mutex_;

	RenderCommandQueue() = default;
	RenderCommandQueue(const RenderCommandQueue&) = delete;
	RenderCommandQueue& operator=(const RenderCommandQueue&) = delete;
};
