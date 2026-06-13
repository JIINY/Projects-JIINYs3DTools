#include "WorldPivotUI.hpp"
#include <DirectXMath.h>
#include <string>
#include "../camera/CameraController.hpp"
#include "../camera/Camera.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;
using namespace DirectX;


void WorldPivotUI::draw(CameraController* activeCamController, const ImVec2& viewportPos, const ImVec2& viewportSize)
{
    if (!activeCamController || !activeCamController->getCamera()) { return; }

    ImVec2 windowPos = ImVec2(viewportPos.x + (axisSize_ * 0.12f), viewportPos.y + viewportSize.y - (axisSize_ * 0.8f));

    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(ImVec2(axisSize_, axisSize_));

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoResize;
    ImGui::Begin("WorldPivot", nullptr, flags);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 windowMin = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 center = ImVec2(windowMin.x + axisSize_ * 0.5f, windowMin.y + axisSize_ * 0.5f);

    auto* cam = activeCamController->getCamera();
    XMMATRIX viewMat = cam->getViewMatrixXM();

    ImGuiIO& io = ImGui::GetIO();

    XMVECTOR axes[3] = {
        XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    };
    ImU32 colors[3] = {
        IM_COL32(255, 50, 50, 255),
        IM_COL32(50, 50, 255, 255),
        IM_COL32(50, 255, 50, 255)
    };
    const char* labels[3] = { "X", "Z", "Y" };

    for (int i = 0; i < 3; ++i) 
    {
        XMVECTOR viewDir = XMVector3TransformNormal(axes[i], viewMat);

        XMFLOAT3 dir;
        XMStoreFloat3(&dir, viewDir);

        ImVec2 endPoint = ImVec2(center.x + dir.x * axisLength_, center.y - dir.y * axisLength_);
        drawList->AddLine(center, endPoint, colors[i], thickness_);

        ImVec2 textPos = ImVec2(
            center.x + dir.x * (axisLength_ + 10.0f) - 4.0f, 
            center.y - dir.y * (axisLength_ + 10.0f) - 10.0f
        );
        drawList->AddText(textPos, colors[i], labels[i]);
    }

    ImGui::End();
}
