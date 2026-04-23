#pragma once
#include "EditorEventVariant.hpp"


class EditorEventPublisher
{
public:
	static EditorEventPublisher& get();

	void publish(const EditorEventVariant& event);

private:
	EditorEventPublisher() = default;
	EditorEventPublisher(const EditorEventPublisher&) = delete;
	EditorEventPublisher& operator=(const EditorEventPublisher&) = delete;
};
