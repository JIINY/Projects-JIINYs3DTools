#pragma once
#include <vector>
#include <memory>
#include "Selectable.hpp"


namespace Selection 
{
    class SelectionList 
    {
    public:
        ~SelectionList();

        void select(std::shared_ptr<Selectable> item);
        void toggle(std::shared_ptr<Selectable> item);
        void remove(std::shared_ptr<Selectable> item);

        void selectRange(const std::vector<std::shared_ptr<Selectable>>& items);
        void appendRange(const std::vector<std::shared_ptr<Selectable>>& items);
        void removeRange(const std::vector<std::shared_ptr<Selectable>>& items);

        void clear();

        bool contains(const Selectable* item) const;
        void setAll(const std::vector<std::shared_ptr<Selectable>>& items);
        const std::vector<std::shared_ptr<Selectable>>& getAll() const { return selection_; }
        std::shared_ptr<Selectable> getLastSelected() const;

        size_t count() const { return selection_.size(); }
        bool isEmpty() const { return selection_.empty(); }


    private:
        std::vector<std::shared_ptr<Selectable>> selection_ = {};

        void pushInternal(std::shared_ptr<Selectable> item); //중복 체크 후 push_back
        void eraseInternal(std::vector<std::shared_ptr<Selectable>>::iterator it); //찾아서 지움
    };
}
