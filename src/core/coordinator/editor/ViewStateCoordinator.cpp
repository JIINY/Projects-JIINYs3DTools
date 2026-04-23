#include "ViewStateCoordinator.hpp"
#include "event/uiEvent/UIEventPublisher.hpp"
#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/viewport/GridChangedEvent.hpp"
#include "event/uiEvent/viewport/GridRequestedEvent.hpp"

void ViewStateCoordinator::initialize()
{
	//콜백 구독 예약
	auto gridSubID = UIEventSubscriber::get().subscribe<GridRequestedEvent>([this](const GridRequestedEvent& event)
		{
			this->onGridChanged(event);
		});
	uiEventSubID_.push_back(gridSubID);
}

void ViewStateCoordinator::onGridChanged(const GridRequestedEvent& event)
{
	if (showGrid_ == event.isVisible && gridSize_ == event.gridSize && gridSpacing_ == event.gridSpacing) { return; }

	showGrid_ = event.isVisible;
	gridSize_ = event.gridSize;
	gridSpacing_ = event.gridSpacing;
	UIEventPublisher::get().publish(GridChangedEvent{ showGrid_, gridSize_, gridSpacing_ });
}

void ViewStateCoordinator::toggleGrid() 
{
	GridRequestedEvent event;
	event.isVisible = !showGrid_;
	event.gridSize = gridSize_;
	event.gridSpacing = gridSpacing_;

	this->onGridChanged(event);
}
