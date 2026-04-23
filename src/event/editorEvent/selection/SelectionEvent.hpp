#pragma once
#include <vector>
#include <memory>

namespace Selection { class Selectable; }


struct SelectionRequestedEvent 
{
    std::shared_ptr<Selection::Selectable> target;
    bool isMultiSelect;
    bool isSubtract;
};

struct SelectionDragRequestedEvent 
{
    std::vector<std::shared_ptr<Selection::Selectable>> targets;
    bool isShift;
    bool isAlt;
};

struct SelectionChangedEvent 
{
    std::vector<std::shared_ptr<Selection::Selectable>> currentSelection;
};
