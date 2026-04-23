#pragma once


struct KeyDownEditorEvent
{
    int keyCode = 0;
    bool isCtrl = false;
    bool isAlt = false;
    bool isShift = false;
};

struct KeyHoldEditorEvent
{
    int keyCode = 0;
    bool isCtrl = false;
    bool isAlt = false;
    bool isShift = false;
};

struct KeyUpEditorEvent
{
    int keyCode = 0;
    bool isCtrl = false;
    bool isAlt = false;
    bool isShift = false;
};
