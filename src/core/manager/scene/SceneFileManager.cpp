#include "SceneFileManager.hpp"
#include <memory>
#include <filesystem>
#include "SceneObjectManager.hpp"
#include "core/PathConfig.hpp"
#include "ui/window/environmentConfig/EnvironmentConfig.hpp"
#include "utils/FileDialogUtils.hpp"

#include "command/CommandStack.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/flow/SceneDataRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentDataChangedEvent.hpp"
#include "event/uiEvent/UIEventPublisher.hpp"
#include "event/uiEvent/message/MessagePopupRequestedEvent.hpp"

#include "common/DebugLog.hpp"
using namespace std;


SceneFileManager::SceneFileManager() : sceneSerializer_(std::make_unique<Scene::SceneSerializer>()) {}
SceneFileManager::~SceneFileManager() = default;

bool SceneFileManager::initialize(SceneFileContext context)
{
    Scene::SceneContext sceneCon;
    sceneCon.sceneObjManager = context.sceneObjManager;
    sceneCon.envConfig = context.envConfig;
    sceneCon.matManager = context.matManager;

    if (!sceneSerializer_->initialize(sceneCon)) { return false; }
    
    auto sceneRequestID = AppEventSubscriber::get().subscribe<SceneDataRequestedEvent>([this](const SceneDataRequestedEvent& event)
        {
            this->onSceneDataRequested(event);
        });
    appEventSubID_.push_back(sceneRequestID);

    auto sceneChangeID = AppEventSubscriber::get().subscribe<SceneDataChangedEvent>([this](const SceneDataChangedEvent& event)
        {
            isDirty_ = true;
        });
    appEventSubID_.push_back(sceneChangeID);

    auto envDataChangeID = AppEventSubscriber::get().subscribe<EnvironmentDataChangedEvent>([this](const EnvironmentDataChangedEvent& event)
        {
            if (isLoading_) { isLoading_ = false; }
            else { this->onEnvironmentConfigChanged(); }
        });
    appEventSubID_.push_back(envDataChangeID);

    return true;
}

void SceneFileManager::onEnvironmentConfigChanged()
{
    isDirty_ = true;
    AppEventPublisher::get().publish(SceneDataChangedEvent{ SceneDataType::DataChanged, currentFilePath_ });
}

void SceneFileManager::onSceneDataRequested(const SceneDataRequestedEvent& event)
{
    switch (event.type)
    {
    case SceneActionType::Save:
    {
        if (currentFilePath_.empty()) { saveAs(); }
        else
        {
            if (sceneSerializer_->serialize(currentFilePath_))
            {
                CommandStack::get().markSaved();
                isDirty_ = false;
                AppEventPublisher::get().publish(SceneDataChangedEvent{ SceneDataType::FileSaved, currentFilePath_ });
            }
        }
        break;
    }
    case SceneActionType::SaveAs:
    {
        saveAs();
        break;
    }
    case SceneActionType::Load:
    {
        string filePath = FileDialogUtils::OpenFile("JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0");
        if (!filePath.empty())
        {
            isLoading_ = true;
            string errorMsg = "";
            if (sceneSerializer_->deserialize(filePath, errorMsg))
            {
                currentFilePath_ = filePath;
                currentFileName_ = filesystem::path(filePath).filename().string();
                CommandStack::get().markSaved();
                isDirty_ = false;
                AppEventPublisher::get().publish(SceneDataChangedEvent{ SceneDataType::FileLoaded, currentFilePath_ });

                if (!errorMsg.empty())
                {
                    UIEventPublisher::get().publish(MessagePopupRequestedEvent{ "Load Failed", errorMsg });
                }
            }
        }
        break;
    }
    case SceneActionType::New:
    {
        isLoading_ = true;
        string errorMsg = "";
        if (sceneSerializer_->deserialize(ScenePathConfig::defaultFilePath_, errorMsg))
        {
            currentFilePath_ = "";
            currentFileName_ = "";
            CommandStack::get().markSaved();
            isDirty_ = false;
            AppEventPublisher::get().publish(SceneDataChangedEvent{ SceneDataType::FileLoaded, "" });

            if (!errorMsg.empty())
            {
                UIEventPublisher::get().publish(MessagePopupRequestedEvent{ "New Scene Failed", errorMsg });
            }
        }
        break;
    }
    default:
        break;
    }
}

void SceneFileManager::saveAs()
{
    string filePath = FileDialogUtils::SaveFile("JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0", "", "json", ScenePathConfig::userFilePath_.c_str());

    if (!filePath.empty())
    {
        if (sceneSerializer_->serialize(filePath))
        {
            currentFilePath_ = filePath;
            currentFileName_ = filesystem::path(filePath).filename().string();
            CommandStack::get().markSaved();
            isDirty_ = false;
            AppEventPublisher::get().publish(SceneDataChangedEvent{ SceneDataType::FileSaved, currentFilePath_ });
        }
    }
}
