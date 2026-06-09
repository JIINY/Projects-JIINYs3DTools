#pragma once
#include <memory>
#include <string>
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/EnvironmentDataChangedEvent.hpp"
#include "core/manager/scene/LightManager.hpp"

class LightManager;
namespace EnvConfig { class LightConfig; }

namespace EnvConfig 
{
    class EnvironmentConfig
    {
    public:
        EnvironmentConfig();
        ~EnvironmentConfig();

        bool initialize(LightManager* manager);
        void draw(bool isVisible);

        LightConfig* getLightConfig() { return lightConfig_.get(); }
        std::string getFileName(const std::string& path);

        const std::string& getCurrentEnvConfigFileName() { return currentFileName_; }
        const std::string& getCurrentEnvConfigFilePath() { return currentFilePath_; }


    private:
        std::vector<AppEventSubscriptionID> appEventSubID_;
        std::unique_ptr<LightConfig> lightConfig_;
        LightManager* lightManager_ = nullptr;

        EnvActionType pendingRequestType_ = EnvActionType::Count;
        std::string currentFileName_ = "";
        std::string currentFilePath_ = "";

        bool isDirty_ = false;
        bool openConfirmDiscardTrigger_ = false;
        
        void onEnvironmentDataChanged(const EnvironmentDataChangedEvent& event);
        void doSaveAs();
        void doLoad();
        static const char* getPopupTitleFromType(const EnvActionType& type);
    };
}
