#pragma once
#include "AppEventVariant.hpp"


class AppEventPublisher
{
public:
	static AppEventPublisher& get();

	void publish(const AppEventVariant& event);

private:
	AppEventPublisher() = default;
	AppEventPublisher(const AppEventPublisher&) = delete;
	AppEventPublisher& operator=(const AppEventPublisher&) = delete;
};
