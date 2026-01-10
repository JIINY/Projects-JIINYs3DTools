#pragma once
#include <string>
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/EnvironmentDataChangedEvent.hpp"
#include "LightConfig.hpp"

class LightManager;
namespace EnvConfig
{
    class EnvironmentConfigSerializer;
}


namespace EnvConfig 
{
    class EnvironmentConfig
    {
    public:
        void initialize(LightManager* manager);
        void draw(bool isVisible, EnvConfig::EnvironmentConfigSerializer* serializer);


    private:
        EnvConfig::LightConfig lightConfig_;
        std::string currentFilePath_ = "";
        std::string defaultFilePath_ = "src/defaults/DefaultEnvironmentConfig.json";
        EnvDataType pendingRequestType_ = EnvDataType::Count;

        std::vector<AppEventSubscriptionID> AppEventSubID_;
        bool isDirty_ = false;
        bool openConfirmDiscardTrigger_ = false;
        
        void onEnvironmentDataChange();
        static const char* getPopupTitleFromType(const EnvDataType& type);
    };
}
