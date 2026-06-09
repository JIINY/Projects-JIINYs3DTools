#pragma once
#include <vector>
#include <memory>
#include "../ICommand.hpp"
#include "common/Math.hpp"
#include "object/SceneObject.hpp"


class CmdScaleSceneObject : public ICommand
{
public:
    CmdScaleSceneObject(std::shared_ptr<SceneObject> target, const Math::Vec3& beginScale, const Math::Vec3& endScale)
        : target_(target), beginScale_(beginScale), endScale_(endScale) {}
    ~CmdScaleSceneObject() override = default;

    void execute() override
    {
        if (!target_) { return; }
        target_->setScale(endScale_);
    }

    void undo() override
    {
        if (!target_) { return; }
        target_->setScale(beginScale_);
    }

    bool emitsDataChanged() const override { return true; }


private:
    std::shared_ptr<SceneObject> target_ = nullptr;
    Math::Vec3 beginScale_ = {};
    Math::Vec3 endScale_ = {};
};
