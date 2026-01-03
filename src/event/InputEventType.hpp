#pragma once
#include <windows.h>
#include <variant>

struct KeyPressedEvent 
{
	int keyCode_;
	bool isRepeat_;
};

struct KeyDownEvent
{
	int keyCode_;
};

struct KeyUpEvent
{
	int keyCode_;
};

struct KeyHoldEvent 
{
	int keyCode_;
	int holdCount_;
};

struct MouseMoveEvent 
{
	float deltaX_, deltaY_;
	POINT pos_;
};

struct MouseDownEvent 
{
	int button_; //0 = left, 1 = right, 2 = middle
	POINT pos_;
};

struct MouseUpEvent
{
	int button_;
	POINT pos_;
};

struct MouseHoldEvent 
{
	int button_;
	POINT pos_;
};

struct MouseWheelEvent 
{
	float delta_;
	POINT pos_;
};

struct ElementIDCalledEvent 
{
	const char* elementID_;
};



using InputEvent = std::variant<
	KeyPressedEvent, KeyDownEvent, KeyUpEvent, KeyHoldEvent, 
	MouseMoveEvent, MouseDownEvent, MouseUpEvent, MouseHoldEvent, MouseWheelEvent,
	ElementIDCalledEvent
>;
