#pragma once
#include <string>
#include <vector>
#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/message/MessagePopupRequestedEvent.hpp"


class MessagePopupController
{
public:
    bool initialize();
    void draw();

private:
    std::vector<UIEventSubscriptionID> uiEventSubID_;
    std::string title_ = "";
    std::string pendingMsg_ = "";
    bool openTrigger_ = false;

    void onMessagePopupRequested(const MessagePopupRequestedEvent& event);
};
