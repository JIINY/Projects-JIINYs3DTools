#pragma once
#include <vector>
#include <memory>
#include "../ICommand.hpp"
#include "common/Math.hpp"
#include "object/SceneObject.hpp"


class CmdMoveSceneObject : public ICommand
{
public:
    CmdMoveSceneObject(std::shared_ptr<SceneObject> target, const Math::Vec3& beginPos, const Math::Vec3& endPos)
        : target_(target), beginPos_(beginPos), endPos_(endPos) {}
    ~CmdMoveSceneObject() override = default;

    void execute() override
    {
        if (!target_) { return; }
        target_->setPosition(endPos_);
    }

    void undo() override
    {
        if (!target_) { return; }
        target_->setPosition(beginPos_);
    }


private:
    std::shared_ptr<SceneObject> target_ = nullptr;
    Math::Vec3 beginPos_ = {};
    Math::Vec3 endPos_ = {};
};
