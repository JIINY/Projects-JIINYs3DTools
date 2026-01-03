#pragma once
#include <memory>
#include <vector>
#include "event/uiEvent/UIEventSubscriber.hpp"


class ViewStateCoordinator 
{
public:
	void initialize();
	void draw() {}

	void toggleGrid();

	void setGridVisibility(bool isVisible);
	bool isGridVisible() const { return isGridVisible_; }


private:
	std::vector<UIEventSubscriptionID> UIEventSubID_;
	bool isGridVisible_ = true;

	void onGridChanged(const GridChangedEvent& event);
};
