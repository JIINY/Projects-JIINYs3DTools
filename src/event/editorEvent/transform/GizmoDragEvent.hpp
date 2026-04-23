#pragma once
#include "common/Mode.hpp"
#include "common/Math.hpp"

enum class GizmoDragState
{
    Begin,
    Update,
    End,
    Count
};

struct GizmoDragEvent
{
    Math::Axis axis = Math::Axis::Count;
    float delta = 0.0f;
    TransformMode mode = TransformMode::Count;
    GizmoDragState state = GizmoDragState::Count;
};
