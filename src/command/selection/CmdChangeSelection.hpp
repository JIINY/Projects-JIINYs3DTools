#pragma once
#include <vector>
#include <memory>
#include "command/ICommand.hpp"
#include "selection/SelectionList.hpp"

#include "event/editorEvent/EditorEventPublisher.hpp"
#include "event/editorEvent/selection/SelectionChangeEvent.hpp"

#include "common/DebugLog.hpp"
namespace Selection { class Selectable; }


class CmdChangeSelection : public ICommand
{
public:
    CmdChangeSelection(Selection::SelectionList* selectionList,
        std::vector<std::shared_ptr<Selection::Selectable>> before, std::vector<std::shared_ptr<Selection::Selectable>> after)
        : selectionList_(selectionList), before_(std::move(before)), after_(std::move(after)) {}
    ~CmdChangeSelection() override = default;

    void execute() override
    {
        selectionList_->setAll(after_);
        EditorEventPublisher::get().publish(SelectionChangedEvent{ after_ });
    }

    void undo() override
    {
        selectionList_->setAll(before_);
        EditorEventPublisher::get().publish(SelectionChangedEvent{ before_ });
    }

    bool emitsDataChanged() const override { return true; }


private:
    Selection::SelectionList* selectionList_ = nullptr;
    std::vector<std::shared_ptr<Selection::Selectable>> before_;
    std::vector<std::shared_ptr<Selection::Selectable>> after_;
};
