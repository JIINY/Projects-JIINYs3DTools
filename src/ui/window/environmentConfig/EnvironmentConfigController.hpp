#pragma once
#include <string>
#include <memory>
#include <vector>
#include "event/appEvent/AppEventSubscriber.hpp"
#include "core/PathConfig.hpp"
#include "core/manager/scene/LightManager.hpp"

struct EnvironmentDataRequestedEvent;
namespace EnvConfig { class EnvironmentConfigSerializer; }


namespace EnvConfig
{
    struct EnvironmentContext
    {
        LightManager* lightManager = nullptr;
    };


    class EnvironmentConfigController
    {
    public:
        EnvironmentConfigController();
        ~EnvironmentConfigController();

        bool initialize(const EnvironmentContext& context);

    private:
        std::vector<AppEventSubscriptionID> appEventSubID_;
        std::unique_ptr<EnvironmentConfigSerializer> envSerializer_;
        LightManager* lightManager_ = nullptr;

        GlobalLightConfig backupGlobalLight_;

        void onEnvironmentRequested(const EnvironmentDataRequestedEvent& event);
    };
}
