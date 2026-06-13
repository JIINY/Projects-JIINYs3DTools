#include "SelectionList.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include "Selectable.hpp"
#include "event/editorEvent/EditorEventPublisher.hpp"
#include "event/editorEvent/selection/SelectionChangeEvent.hpp"
#include "common/DebugLog.hpp"
using namespace std;


namespace Selection 
{
    SelectionList::~SelectionList() 
    {
        clear();
    }

    void SelectionList::select(std::shared_ptr<Selectable> item) 
    {
        if (!item) { return; }
        if (selection_.size() == 1 && selection_[0] == item) { return; }

        clear();
        pushInternal(item);

        EditorEventPublisher::get().publish(SelectionChangedEvent{ selection_ });
    }

    void SelectionList::selectRange(const vector<shared_ptr<Selectable>>& items) 
    {
        clear();
        for (const auto& it : items) 
        {
            pushInternal(it);
        }

        EditorEventPublisher::get().publish(SelectionChangedEvent{ selection_ });
    }

    void SelectionList::appendRange(const vector<shared_ptr<Selectable>>& items) 
    {
        if (items.empty()) { return; }

        auto last = getLastSelected();
        if (last && last->getSelectableType() != items.front()->getSelectableType()) 
        {
            clear();
        }

        bool changed = false;
        for (const auto& it : items)
        {
            if (!contains(it.get())) 
            {
                pushInternal(it);
                changed = true;
            }
        }

        if (changed) 
        {
            EditorEventPublisher::get().publish(SelectionChangedEvent{ selection_ });
        }
    }

    void SelectionList::removeRange(const vector<shared_ptr<Selectable>>& items) 
    {
        if (selection_.empty()) { return; }

        bool changed = false;
        for (const auto& item : items) 
        {
            auto it = find(selection_.begin(), selection_.end(), item);
            if (it != selection_.end()) 
            {
                eraseInternal(it);
                changed = true;
            }
        }

        if (changed)
        {
            EditorEventPublisher::get().publish(SelectionChangedEvent{ selection_ });
        }
    }

    void SelectionList::remove(shared_ptr<Selectable> item) 
    {
        if (!item) { return; }
        auto it = find(selection_.begin(), selection_.end(), item);

        if (it != selection_.end()) 
        {
            eraseInternal(it);
            EditorEventPublisher::get().publish(SelectionChangedEvent{ selection_ });
        }
    }

    void SelectionList::clear() 
    {
        for (auto& it : selection_)
        {
            if (it) { it->onDeselected(); }
        }
        selection_.clear();

        EditorEventPublisher::get().publish(SelectionChangedEvent{ selection_ });
    }

    bool SelectionList::contains(const Selectable* item) const 
    {
        if (!item) { return false; }
        for (const auto& ptr : selection_) 
        {
            if (ptr.get() == item) { return true; }
        }
        return false;
    }

    shared_ptr<Selectable> SelectionList::getLastSelected() const 
    {
        if (selection_.empty()) { return nullptr; }
        return selection_.back();
    }

    void SelectionList::pushInternal(shared_ptr<Selectable> item) 
    {
        if (item) 
        {
            selection_.push_back(item);
            item->onSelected();
        }
    }

    void SelectionList::eraseInternal(vector<shared_ptr<Selectable>>::iterator it) 
    {
        if (it != selection_.end()) 
        {
            (*it)->onDeselected();
            selection_.erase(it);
        }
    }

    void SelectionList::setAll(const vector<shared_ptr<Selectable>>& items)
    {
        for (auto& o : selection_)
        {
            if (o) { o->onDeselected(); }
        }
        selection_.clear();

        for (const auto& item : items)
        {
            pushInternal(item);
        }
    }
}
