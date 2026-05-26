#pragma once
#include <vector>
#include <memory>
#include "selection/SelectableType.hpp"

namespace Selection { class Selectable; }


struct SelectionStateRequestedEvent {};

struct SelectionStateProvidedEvent
{
    int selectionCount = 0;
    Selection::SelectableType currentType = Selection::SelectableType::Count;
    std::vector<std::shared_ptr<Selection::Selectable>> currentSelection;
};
