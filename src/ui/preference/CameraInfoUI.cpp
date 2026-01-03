#include "CameraInfoUI.hpp"
#include "core/manager/editor/ViewportCameraManager.hpp"
#include "viewport/camera/Camera.hpp"
#include "viewport/camera/CameraController.hpp"
#include "event/uiEvent/UIEventPublisher.hpp"
#include "event/uiEvent/viewport/CameraInfoRequestedEvent.hpp"
#include "imgui.h"
using namespace std;


void CameraInfoUI::initialize(ViewportCameraManager* manager) 
{
	manager_ = manager;
}


void CameraInfoUI::draw(bool isVisible) 
{
	if (!isVisible) { return; }
	bool isOpen = true;

	if (!ImGui::Begin("Camera Info", &isOpen))
	{
		ImGui::End();
		return;
	}

	ImGui::Text("Camera State");

	if (manager_) 
	{
		Camera camera = manager_->getActiveCam();
		auto pos = camera.getPosition();
		auto target = camera.getTarget();
		auto up = camera.getUpDirection();

		ImGui::Text("Pos   : [%.2f, %.2f, %.2f]", pos.x, pos.y, pos.z);
		ImGui::Text("Target: [%.2f, %.2f, %.2f]", target.x, target.y, target.z);
		ImGui::Text("Up Vec: [%.2f, %.2f, %.2f]", up.x, up.y, up.z);
		ImGui::Separator();
		ImGui::Text("FOV : %.1f", DirectX::XMConvertToDegrees(camera.getFovY()));
		ImGui::Text("Near: %.2f", camera.getNearZ());
		ImGui::Text("Far : %.1f", camera.getFarZ());
	}
	else
	{
		ImGui::Text("Pos   : -");
		ImGui::Text("Target: -");
		ImGui::Text("Up Vec: -");
		ImGui::Separator();
		ImGui::Text("FOV : -");
		ImGui::Text("Near: -");
		ImGui::Text("Far : -");
	}

	ImGui::Separator();
	CameraController* controller = nullptr;
	if (manager_) 
	{
		controller = manager_->getActiveCamController();
	}

	if (controller) 
	{
		ImGui::Text("Dist : %.2f", controller->getDistance());
		ImGui::Text("Pitch: %.2f", controller->getPitch());
		ImGui::Text("Yaw  : %.2f", controller->getYaw());
	}
	else 
	{
		ImGui::Text("Dist : -");
		ImGui::Text("Pitch: -");
		ImGui::Text("Yaw  : -");
	}

	ImGui::End();

	if (!isOpen) 
	{
		UIEventPublisher::get().publish(CameraInfoRequestedEvent{ false });
	}
}
