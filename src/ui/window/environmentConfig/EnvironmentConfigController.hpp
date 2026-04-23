#pragma once
#include <string>
#include <memory>
#include <vector>
#include "event/appEvent/AppEventSubscriber.hpp"
#include "core/PathConfig.hpp"
#include "core/manager/scene/LightManager.hpp"
#include "EnvironmentConfigSerializer.hpp"


struct EnvironmentDataRequestedEvent;

namespace EnvConfig
{
    struct EnvironmentContext
    {
        LightManager* lightManager = nullptr;
    };


    class EnvironmentConfigController
    {
    public:
        bool initialize(const EnvironmentContext& context);

    private:
        std::vector<AppEventSubscriptionID> appEventSubID_;
        LightManager* lightManager_ = nullptr;
        EnvConfig::EnvironmentConfigSerializer envSerializer_;

        GlobalLightConfig backupGlobalLight_;

        void onEnvironmentRequested(const EnvironmentDataRequestedEvent& event);
    };
}
