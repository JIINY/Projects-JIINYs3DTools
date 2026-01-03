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
			this->setGridVisibility(event.isVisible);
		});
	UIEventSubID_.push_back(gridSubID);
}

void ViewStateCoordinator::setGridVisibility(bool isVisible) 
{
	if (isGridVisible_ == isVisible) { return; }

	isGridVisible_ = isVisible;
	UIEventPublisher::get().publish(GridChangedEvent{ isGridVisible_ });
}

void ViewStateCoordinator::toggleGrid() 
{
	setGridVisibility(!isGridVisible_);
}
