#pragma once
#include "UIEventVariant.hpp"


class UIEventPublisher
{
public:
	static UIEventPublisher& get();

	void publish(const UIEventVariant& event);

private:
	UIEventPublisher() = default;
	UIEventPublisher(const UIEventPublisher&) = delete;
	UIEventPublisher& operator=(const UIEventPublisher&) = delete;
};
