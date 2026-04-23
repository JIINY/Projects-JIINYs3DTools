#pragma once
#include <vector>
#include <memory>
#include "../ICommand.hpp"
#include "core/coordinator/scene/PassiveObjectCoordinator.hpp"
#include "render/primitives/PrimitivesType.hpp"

class SceneObject;

class CmdDeletePrimitive : public ICommand
{
public:
    CmdDeletePrimitive(PassiveObjectCoordinator* coordinator, const std::vector<std::shared_ptr<SceneObject>>& targets) 
        : passiveObjCoord_(coordinator), passiveObjects_(targets) {}

    ~CmdDeletePrimitive() override = default;

    void execute() override 
    {
        for (auto& obj : passiveObjects_) 
        {
            if (obj) 
            {
                passiveObjCoord_->removeObject(obj);
            }
        }
    }

    void undo() override 
    {
        for (auto& obj : passiveObjects_) 
        {
            if (obj) 
            {
                passiveObjCoord_->addObject(obj);
            }
        }
    }


private:
    PassiveObjectCoordinator* passiveObjCoord_ = nullptr;
    std::vector<std::shared_ptr<SceneObject>> passiveObjects_;
};
