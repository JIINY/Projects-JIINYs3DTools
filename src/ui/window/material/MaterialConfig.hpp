#pragma once
#include <string>
#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/material/MaterialTargetChangedEvent.hpp"

class SceneObject;
namespace Render { class Material; }

namespace MaterialEditor
{
    class MaterialConfig
    {
    public:
        bool initialize();
        void draw();


    private:
        std::vector<UIEventSubscriptionID> uiEventSubID_;
        std::shared_ptr<Render::Material> targetMat_ = nullptr;
        std::string targetName_;

        void onMaterialTargetChanged(const MaterialTargetChangedEvent& event);
    };
}
