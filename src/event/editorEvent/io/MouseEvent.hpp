#pragma once
#include <windows.h>
#include "common/Math.hpp"


struct MouseDownEditorEvent
{
    int button_ = 0;
    POINT pos_ = {};
    Math::Ray mouseRay_ = {};

    bool isCtrl = false;
    bool isAlt = false;
    bool isShift = false;
};

struct MouseHoldEditorEvent
{
    int button_ = 0;
    float deltaX_ = 0.0f;
    float deltaY_ = 0.0f;
    POINT pos_ = {};
    Math::Ray mouseRay_ = {};

    bool isCtrl = false;
    bool isAlt = false;
    bool isShift = false;
};

struct MouseUpEditorEvent
{
    int button_ = 0;
    POINT pos_ = {};
    Math::Ray mouseRay_ = {};

    bool isCtrl = false;
    bool isAlt = false;
    bool isShift = false;
};

struct MouseWheelEditorEvent
{
    float delta_ = 0.0f;
    POINT pos_ = {};
    Math::Ray mouseRay_ = {};

    bool isCtrl = false;
    bool isAlt = false;
    bool isShift = false;
};

struct MouseMoveEditorEvent
{
    float deltaX_ = 0.0f;
    float deltaY_ = 0.0f;
    POINT pos_ = {};
    Math::Ray mouseRay_ = {};

    bool isCtrl = false;
    bool isAlt = false;
    bool isShift = false;
};
