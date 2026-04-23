#pragma once


enum class SelectionMode
{
    Object,
    AnimationKey,
    Count
};

struct SelectionModeChangedEvent 
{
    SelectionMode newMode;

    SelectionModeChangedEvent(SelectionMode mode) : newMode(mode) {}
};
