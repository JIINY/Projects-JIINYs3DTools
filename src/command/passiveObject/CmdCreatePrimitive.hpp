#pragma once
#include <memory>
#include "../ICommand.hpp"
#include "core/coordinator/scene/PassiveObjectCoordinator.hpp"
#include "render/primitives/PrimitivesType.hpp"

#include "common/DebugLog.hpp"

class SceneObject;

class CmdCreatePrimitive : public ICommand 
{
public:
    CmdCreatePrimitive(PassiveObjectCoordinator* coordinator, const Render::Primitives::PrimitiveData& data)
        : passiveObjCoord_(coordinator), data_(data), targetObject_(nullptr) {}
    
    ~CmdCreatePrimitive() override = default;

    void execute() override 
    {
        if (!targetObject_)
        {
            targetObject_ = passiveObjCoord_->createPrimitive(data_);
        }
        else //redo
        {
            passiveObjCoord_->addObject(targetObject_);
        }
    }

    void undo() override 
    {
        if (targetObject_)
        {
            passiveObjCoord_->removeObject(targetObject_);
        }
    }

    bool emitsDataChanged() const override { return true; }


private:
    PassiveObjectCoordinator* passiveObjCoord_ = nullptr;
    Render::Primitives::PrimitiveData data_ = {};
    std::shared_ptr<SceneObject> targetObject_ = nullptr;
};
