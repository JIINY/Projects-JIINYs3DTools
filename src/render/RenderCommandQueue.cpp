#include "RenderCommandQueue.hpp"

RenderCommandQueue& RenderCommandQueue::get() 
{
	static RenderCommandQueue instance;
	return instance;
}
