#pragma once
#include <vector>
#include <memory>
#include "../ICommand.hpp"
#include "common/Math.hpp"
#include "object/SceneObject.hpp"


class CmdRotateSceneObject : public ICommand
{
public:
    CmdRotateSceneObject(std::shared_ptr<SceneObject> target, const Math::Vec3& beginRot, const Math::Vec3& endRot)
        : target_(target), beginRot_(beginRot), endRot_(endRot) {}
    ~CmdRotateSceneObject() override = default;

    void execute() override
    {
        if (!target_) { return; }
        target_->setRotation(endRot_);
    }

    void undo() override
    {
        if (!target_) { return; }
        target_->setRotation(beginRot_);
    }


private:
    std::shared_ptr<SceneObject> target_ = nullptr;
    Math::Vec3 beginRot_ = {};
    Math::Vec3 endRot_ = {};
};
