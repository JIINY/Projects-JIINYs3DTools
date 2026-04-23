#pragma once
#include <memory>
#include <vector>
#include "event/uiEvent/UIEventSubscriber.hpp"


class ViewStateCoordinator 
{
public:
	ViewStateCoordinator() = default;
	~ViewStateCoordinator() = default;

	void initialize();
	void draw() {}

	void toggleGrid();
	bool isGridVisible() const { return showGrid_; }


private:
	std::vector<UIEventSubscriptionID> uiEventSubID_;
	bool showGrid_ = true;
	int gridSize_ = 10;
	float gridSpacing_ = 1.0f;

	void onGridChanged(const GridRequestedEvent& event);
};
