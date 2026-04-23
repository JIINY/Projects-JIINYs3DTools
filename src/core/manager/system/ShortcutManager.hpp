#pragma once
#include "event/editorEvent/EditorEventSubscriber.hpp"
#include "event/editorEvent/io/KeyboardEvent.hpp"


class ShortcutManager 
{
public:
	ShortcutManager() = default;
	~ShortcutManager() = default;

	bool initialize();


private:
	std::vector<EditorEventSubscriptionID> editorEventSubID_;

	void onKeyDowned(const KeyDownEditorEvent& event);
};
