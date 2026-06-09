#include "CommandStack.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/flow/SceneDataChangedEvent.hpp"

#include "common/DebugLog.hpp"
using namespace std;


CommandStack& CommandStack::get()
{
    static CommandStack instance;
    return instance;
}

void CommandStack::execute(std::shared_ptr<ICommand> cmd)
{
    lock_guard<mutex> lock(mutex_);
    if (currentCmdIndex_ < cmdStack_.size()) //Undo 여러 번 후 미래 기록들을 삭제
    {
        cmdStack_.resize(currentCmdIndex_);
        if (savedCmdIndex_ > currentCmdIndex_)
        {
            savedCmdIndex_ = INVALID_SAVE_INDEX;
        }
    }

    bool emits = cmd->emitsDataChanged();
    cmd->execute();

    cmdStack_.push_back(std::move(cmd));
    currentCmdIndex_++;

    if (emits)
    {
        AppEventPublisher::get().publish(SceneDataChangedEvent(SceneDataType::DataChanged, ""));
    }
}

void CommandStack::undo()
{
    lock_guard<mutex> lock(mutex_);
    if (currentCmdIndex_ > 0)
    {
        currentCmdIndex_--;
        bool emits = cmdStack_[currentCmdIndex_]->emitsDataChanged();
        cmdStack_[currentCmdIndex_]->undo();

        if (emits)
        {
            AppEventPublisher::get().publish(SceneDataChangedEvent(SceneDataType::DataChanged, ""));
        }
    }
}

void CommandStack::redo()
{
    lock_guard<mutex> lock(mutex_);
    if (currentCmdIndex_ < cmdStack_.size())
    {
        bool emits = cmdStack_[currentCmdIndex_]->emitsDataChanged();
        cmdStack_[currentCmdIndex_]->execute();
        currentCmdIndex_++;

        if (emits)
        {
            AppEventPublisher::get().publish(SceneDataChangedEvent(SceneDataType::DataChanged, ""));
        }
    }
}

void CommandStack::markSaved()
{
    lock_guard<mutex> lock(mutex_);
    savedCmdIndex_ = currentCmdIndex_;
}

bool CommandStack::isDirty() const
{
    lock_guard<mutex> lock(mutex_);
    return currentCmdIndex_ != savedCmdIndex_;
}

void CommandStack::clear()
{
    std::lock_guard<std::mutex> lock(mutex_);
    cmdStack_.clear();
    currentCmdIndex_ = 0;
    savedCmdIndex_ = 0;
}
