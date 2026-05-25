#include "MainMenuBarUI.hpp"
#include <cstdlib>
#include <functional>
#include "common/Mode.hpp"
#include "common/Fonts.hpp"
#include "command/CommandStack.hpp"

#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/state/AppModeChangedEvent.hpp"
#include "event/appEvent/state/CameraModeChangedEvent.hpp"
#include "event/appEvent/ui/CreatePopupChangedEvent.hpp"
#include "event/appEvent/ui/EnvironmentConfigPopupChangedEvent.hpp"
#include "event/appEvent/ui/MaterialPopupChangedEvent.hpp"

#include "event/uiEvent/UIEventPublisher.hpp"
#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/viewport/CameraInfoChangedEvent.hpp"
#include "event/uiEvent/viewport/GridChangedEvent.hpp"
#include "event/uiEvent/viewport/WireframeChangedEvent.hpp"
#include "event/uiEvent/viewport/SolidChangedEvent.hpp"

#include "imgui.h"
#include "common/DebugLog.hpp"


void MainMenuBarUI::initialize() 
{
	AppEventSubscriber::get().subscribe<AppModeChangedEvent>([this](const AppModeChangedEvent& event)
		{
			this->onAppModeChanged(event);
		});
	AppEventSubscriber::get().subscribe<CameraModeChangedEvent>([this](const CameraModeChangedEvent& event)
		{
			this->onCameraModeChanged(event);
		});
	AppEventSubscriber::get().subscribe<CreatePopupChangedEvent>([this](const CreatePopupChangedEvent& event)
		{
			this->onCreatePopupChanged(event);
		});
	AppEventSubscriber::get().subscribe<EnvironmentConfigPopupChangedEvent>([this](const EnvironmentConfigPopupChangedEvent& event)
		{
			this->onEnvironmentConfigPopupChanged(event);
		});

	UIEventSubscriber::get().subscribe<CameraInfoChangedEvent>([this](const CameraInfoChangedEvent& event)
		{
			this->onCameraInfoChanged(event);
		});
	UIEventSubscriber::get().subscribe<GridChangedEvent>([this](const GridChangedEvent& event)
		{
			this->onGridChanged(event);
		});
	UIEventSubscriber::get().subscribe<WireframeChangedEvent>([this](const WireframeChangedEvent& event)
		{
			this->onWireframeChanged(event);
		});
	UIEventSubscriber::get().subscribe<SolidChangedEvent>([this](const SolidChangedEvent& event)
		{
			this->onSolidChanged(event);
		});
}

void MainMenuBarUI::onAppModeChanged(const AppModeChangedEvent& event)
{
	currentMode_ = event.newMode_;
}

void MainMenuBarUI::onCameraModeChanged(const CameraModeChangedEvent& event) 
{
	this->updateCamUIState(event.newCam_, event.newView_);
}

void MainMenuBarUI::onCreatePopupChanged(const CreatePopupChangedEvent& event) 
{
	this->isCreateVisible_ = event.isVisible;
}

void MainMenuBarUI::onEnvironmentConfigPopupChanged(const EnvironmentConfigPopupChangedEvent& event)
{
	this->isEnvironmentConfigVisible_ = event.isVisible;
}

void MainMenuBarUI::onMaterialPopupChanged(const MaterialPopupChangedEvent& event)
{
	this->isMaterialVisible_ = event.isVisible;
}

void MainMenuBarUI::onCameraInfoChanged(const CameraInfoChangedEvent& event) 
{
	this->isCamInfoVisible_ = event.isVisible;
}

void MainMenuBarUI::onGridChanged(const GridChangedEvent& event) 
{
	this->isGridVisible_ = event.isVisible;
}

void MainMenuBarUI::onWireframeChanged(const WireframeChangedEvent& event) 
{
	this->isWireframeVisible_ = event.isVisible;
}

void MainMenuBarUI::onSolidChanged(const SolidChangedEvent& event) 
{
	this->isSolidVisible_ = event.isVisible;
}


void MainMenuBarUI::draw() 
{
	drawMenuBar();
}

void MainMenuBarUI::drawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		fileMenu();
		editMenu();
		modeMenu();
		cameraMenu();
		windowMenu();
		prefMenu();

		ImGui::EndMainMenuBar();
	}
}


void MainMenuBarUI::fileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New File", "Ctrl + N")) {}
		if (ImGui::MenuItem("Open File", "Ctrl + O")) {}
		if (ImGui::MenuItem("Save File", "Ctrl + S")) {}
		ImGui::Separator();
		if (ImGui::MenuItem("Exit", "Ctrl + Q")) { exit(0); }
		ImGui::EndMenu();
	}
}

void MainMenuBarUI::editMenu() 
{
	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Undo", "Ctrl + Z")) { CommandStack::get().undo(); }
		if (ImGui::MenuItem("Redo", "Ctrl + Y")) { CommandStack::get().redo(); }
		ImGui::EndMenu();
	}
}

void MainMenuBarUI::modeMenu() 
{
	if (ImGui::BeginMenu("Mode"))
	{
		if (ImGui::MenuItem("Edit Mode", "Tab", currentMode_ == AppMode::Edit))
		{
			AppMode oldMode = currentMode_;
			AppEventPublisher::get().publish(AppModeSetRequestedEvent(AppMode::Edit));
		}
		if (ImGui::MenuItem("Play Mode", "Tab", currentMode_ == AppMode::Play))
		{
			AppMode oldMode = currentMode_;
			AppEventPublisher::get().publish(AppModeSetRequestedEvent(AppMode::Play));
		}
		ImGui::EndMenu();
	}
}

void MainMenuBarUI::cameraMenu() 
{
	if (ImGui::BeginMenu("Camera"))
	{
		auto publishCamEvent = [&](CameraMode cam, ViewMode view)
		{
			AppEventPublisher::get().publish(CameraModeChangedEvent(cam, view));
		};

		bool disableCamChange = (currentMode_ == AppMode::Play);
		if (disableCamChange) { ImGui::BeginDisabled(true); }

		if (ImGui::RadioButton("Arm Camera", (int*)&combinedCamUI_, (int)CameraUIMode::FreeArm))
			publishCamEvent(CameraMode::FreeArm, ViewMode::None);
		if (ImGui::RadioButton("Target Camera", (int*)&combinedCamUI_, (int)CameraUIMode::Target))
			publishCamEvent(CameraMode::Target, ViewMode::None);
		if (ImGui::RadioButton("Free Camera", (int*)&combinedCamUI_, (int)CameraUIMode::FreeFly))
			publishCamEvent(CameraMode::FreeFly, ViewMode::None);

		ImGui::Separator();

		if (ImGui::RadioButton("TopView", (int*)&combinedCamUI_, (int)CameraUIMode::TopView))
			publishCamEvent(CameraMode::FixedAngle, ViewMode::TopView);
		if (ImGui::RadioButton("FrontView", (int*)&combinedCamUI_, (int)CameraUIMode::FrontView))
			publishCamEvent(CameraMode::FixedAngle, ViewMode::FrontView);
		if (ImGui::RadioButton("LeftView", (int*)&combinedCamUI_, (int)CameraUIMode::LeftView))
			publishCamEvent(CameraMode::FixedAngle, ViewMode::LeftView);
		if (ImGui::RadioButton("RightView", (int*)&combinedCamUI_, (int)CameraUIMode::RightView))
			publishCamEvent(CameraMode::FixedAngle, ViewMode::RightView);
		if (ImGui::RadioButton("BackView", (int*)&combinedCamUI_, (int)CameraUIMode::BackView))
			publishCamEvent(CameraMode::FixedAngle, ViewMode::BackView);
		if (ImGui::RadioButton("BottomView", (int*)&combinedCamUI_, (int)CameraUIMode::BottomView))
			publishCamEvent(CameraMode::FixedAngle, ViewMode::BottomView);

		if (disableCamChange) { ImGui::EndDisabled(); }
		ImGui::EndMenu();
	}
}

void MainMenuBarUI::updateCamUIState(CameraMode cam, ViewMode view) 
{
	if (cam != CameraMode::FixedAngle) 
	{
		switch (cam) {
		case CameraMode::FreeArm: this->combinedCamUI_ = CameraUIMode::FreeArm; break;
		case CameraMode::Target: this->combinedCamUI_ = CameraUIMode::Target; break;
		case CameraMode::FreeFly: this->combinedCamUI_ = CameraUIMode::FreeFly; break;
		}
	}
	else 
	{
		switch (view) {
		case ViewMode::TopView: this->combinedCamUI_ = CameraUIMode::TopView; break;
		case ViewMode::FrontView: this->combinedCamUI_ = CameraUIMode::FrontView; break;
		case ViewMode::LeftView: this->combinedCamUI_ = CameraUIMode::LeftView; break;
		case ViewMode::RightView: this->combinedCamUI_ = CameraUIMode::RightView; break;
		case ViewMode::BackView: this->combinedCamUI_ = CameraUIMode::BackView; break;
		case ViewMode::BottomView: this->combinedCamUI_ = CameraUIMode::BottomView; break;
		}
	}
}

void MainMenuBarUI::windowMenu() 
{
	if (ImGui::BeginMenu("Window")) 
	{
		if (ImGui::MenuItem("Create", nullptr)) 
		{
			AppEventPublisher::get().publish(CreatePopupRequestedEvent{ !isCreateVisible_ });
		}
		if (ImGui::MenuItem("Environment Config", nullptr)) 
		{
			AppEventPublisher::get().publish(EnvironmentConfigPopupRequestedEvent{ !isEnvironmentConfigVisible_ });
		}

		if (ImGui::MenuItem("Material Editor", nullptr))
		{
			AppEventPublisher::get().publish(MaterialPopupRequestedEvent{ !isMaterialVisible_ });
		}
		ImGui::EndMenu();
	}
}

void MainMenuBarUI::prefMenu() 
{
	if (ImGui::BeginMenu("Pref")) 
	{
		if (ImGui::MenuItem("WireView", nullptr, isWireframeVisible_))
		{
			UIEventPublisher::get().publish(WireframeChangedEvent{ !isWireframeVisible_ });
		}
		if (ImGui::MenuItem("SolidView", nullptr, isSolidVisible_)) 
		{
			UIEventPublisher::get().publish(SolidChangedEvent{ !isSolidVisible_ });
		}
		if (ImGui::MenuItem("Grid", nullptr, isGridVisible_)) 
		{
			UIEventPublisher::get().publish(GridRequestedEvent{ !isGridVisible_ });
		}
		if (ImGui::MenuItem("Camera Info", nullptr, isCamInfoVisible_)) 
		{
			UIEventPublisher::get().publish(CameraInfoRequestedEvent{ !isCamInfoVisible_ });
		}

		ImGui::EndMenu();
	}
}
