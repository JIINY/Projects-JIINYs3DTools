#pragma once
#include "common/Mode.hpp"


struct AppModeChangedEvent 
{
	AppMode newMode_;
	AppMode oldMode_;

	AppModeChangedEvent(AppMode newMode, AppMode oldMode) : newMode_(newMode), oldMode_(oldMode) {}
	//생성자(Constructor) 함수, AppModeChangedEvent의 생성자를 정의
};
