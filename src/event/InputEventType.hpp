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

	bool isCtrl_;
	bool isAlt_;
	bool isShift_;
};

struct KeyUpEvent
{
	int keyCode_;

	bool isCtrl_;
	bool isAlt_;
	bool isShift_;
};

struct KeyHoldEvent 
{
	int keyCode_;
	int holdCount_;

	bool isCtrl_;
	bool isAlt_;
	bool isShift_;
};

struct MouseMoveEvent 
{
	float deltaX_, deltaY_;
	POINT pos_;

	bool isCtrl_;
	bool isAlt_;
	bool isShift_;
};

struct MouseDownEvent 
{
	int button_; //0 = left, 1 = right, 2 = middle
	POINT pos_;

	bool isCtrl_;
	bool isAlt_;
	bool isShift_;
};

struct MouseUpEvent
{
	int button_;
	POINT pos_;

	bool isCtrl_;
	bool isAlt_;
	bool isShift_;
};

struct MouseHoldEvent 
{
	int button_;
	float deltaX_, deltaY_;
	POINT pos_;

	bool isCtrl_;
	bool isAlt_;
	bool isShift_;
};

struct MouseWheelEvent 
{
	float delta_;
	POINT pos_;

	bool isCtrl_;
	bool isAlt_;
	bool isShift_;
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
