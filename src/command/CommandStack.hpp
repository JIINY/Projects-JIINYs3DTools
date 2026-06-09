#pragma once
#include <vector>
#include <memory>
#include <mutex>
#include <limits>
#include "ICommand.hpp"


class CommandStack 
{
public:
    static CommandStack& get();

    void execute(std::shared_ptr<ICommand> cmd);
    void undo();
    void redo();

    void markSaved();
    bool isDirty() const;
    void clear();


private:
    static constexpr size_t INVALID_SAVE_INDEX = SIZE_MAX;

    std::vector<std::shared_ptr<ICommand>> cmdStack_;
    size_t currentCmdIndex_ = 0;
    size_t savedCmdIndex_ = 0;
    mutable std::mutex mutex_;

    CommandStack() = default;
    CommandStack(const CommandStack&) = delete;
    CommandStack& operator=(const CommandStack&) = delete;
};
