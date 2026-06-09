#pragma once
#include <memory>
#include "event/editorEvent/EditorEventSubscriber.hpp"

struct SelectionChangedEvent;
struct SelectionStateProvidedEvent;
class MaterialManager;
class SceneObject;
namespace MaterialEditor
{
    class MaterialContainer;
    class MaterialConfig;
}


namespace MaterialEditor
{
    class MaterialPanel
    {
    public:
        MaterialPanel();
        ~MaterialPanel();

        bool initialize(MaterialManager* matManager);
        void draw(bool isVisible);

        std::shared_ptr<SceneObject> convertSceneObject(const std::shared_ptr<Selection::Selectable>& sel) const;
        void applySelection(const std::vector<std::shared_ptr<Selection::Selectable>>& selection);


    private:
        std::vector<EditorEventSubscriptionID> editorEventSubID_;
        std::unique_ptr<MaterialContainer> matContainer_;
        std::unique_ptr<MaterialConfig> matConfig_;

        int currentSelectionSize_ = 0;
        std::shared_ptr<SceneObject> lastSelected_ = nullptr;

        void onSelectionChanged(const SelectionChangedEvent& event);
        void onSelectionStateProvided(const SelectionStateProvidedEvent& event);
    };
}
