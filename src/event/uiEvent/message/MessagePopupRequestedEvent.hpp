#pragma once
#include <string>


struct MessagePopupRequestedEvent
{
	std::string title = "";
	std::string message = "";
};
