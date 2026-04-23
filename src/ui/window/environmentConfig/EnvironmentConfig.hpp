#pragma once
#include <string>
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/EnvironmentDataChangedEvent.hpp"
#include "LightConfig.hpp"
#include "core/manager/scene/LightManager.hpp"

class LightManager;


namespace EnvConfig 
{
    class EnvironmentConfig
    {
    public:
        bool initialize(LightManager* manager);
        void draw(bool isVisible);

        EnvConfig::LightConfig* getLightConfig() { return &lightConfig_; }
        std::string getFileName(const std::string& path);

    private:
        LightManager* lightManager_ = nullptr;
        EnvConfig::LightConfig lightConfig_;
        EnvActionType pendingRequestType_ = EnvActionType::Count;
        std::string currentFileName_ = "";
        std::string currentFilePath_ = "";

        std::vector<AppEventSubscriptionID> appEventSubID_;
        bool isDirty_ = false;
        bool openConfirmDiscardTrigger_ = false;
        
        void onEnvironmentDataChanged(const EnvironmentDataChangedEvent& event);
        void doSaveAs();
        void doLoad();
        static const char* getPopupTitleFromType(const EnvActionType& type);
    };
}
