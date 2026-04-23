#pragma once
#include <vector>
#include <memory>
#include "ICommand.hpp"

#include "common/DebugLog.hpp"

class CommandStack 
{
public:
    static CommandStack& get() 
    {
        static CommandStack instance;
        return instance;
    }

    void execute(std::shared_ptr<ICommand> cmd) 
    {
        if (currentCmdIndex_ < cmdStack_.size()) //Undo 여러 번 후 미래 기록들을 삭제
        {
            cmdStack_.resize(currentCmdIndex_);
        }

        cmd->execute();

        cmdStack_.push_back(std::move(cmd));
        currentCmdIndex_++;
    }

    void undo() 
    {
        if (currentCmdIndex_ > 0) 
        {
            currentCmdIndex_--;
            cmdStack_[currentCmdIndex_]->undo();
        }
    }

    void redo() 
    {
        if (currentCmdIndex_ < cmdStack_.size()) 
        {
            cmdStack_[currentCmdIndex_]->execute();
            currentCmdIndex_++;
        }
    }


private:
    std::vector<std::shared_ptr<ICommand>> cmdStack_;
    size_t currentCmdIndex_ = 0;
    mutable std::mutex mutex_;

    CommandStack() = default;
    CommandStack(const CommandStack&) = delete;
    CommandStack& operator=(const CommandStack&) = delete;
};
