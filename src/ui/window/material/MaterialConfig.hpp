#pragma once
#include <string>
#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/material/MaterialTargetChangedEvent.hpp"

class SceneObject;
class MaterialManager;
namespace Render { class Material; }


namespace MaterialEditor
{
    class MaterialConfig
    {
    public:
        bool initialize(MaterialManager* matManager);
        void draw();


    private:
        std::vector<UIEventSubscriptionID> uiEventSubID_;
        std::shared_ptr<SceneObject> targetObj_ = nullptr;
        std::shared_ptr<Render::Material> targetMat_ = nullptr;
        std::string targetName_;

        MaterialManager* matManager_ = nullptr;
        
        void onMaterialTargetChanged(const MaterialTargetChangedEvent& event);
    };
}
