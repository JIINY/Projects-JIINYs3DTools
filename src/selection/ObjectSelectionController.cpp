#include "ObjectSelectionController.hpp"
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>
#include "core/coordinator/editor/CameraCoordinator.hpp"
#include "core/manager/editor/ViewportCameraManager.hpp"
#include "viewport/camera/Camera.hpp"
#include "object/SceneObject.hpp"

#include "event/EditorEvent/EditorEventPublisher.hpp"
#include "event/EditorEvent/selection/SelectionEvent.hpp"

#include "common/DebugLog.hpp"
using namespace std;
using namespace DirectX;


bool ObjectSelectionController::initialize(CameraCoordinator* camCoord) 
{
    assert(camCoord && "비었습니다. 초기화 실패");
    if (!camCoord) { return false; }
    
    viewportManager_ = camCoord->getViewportCameraManager();
    assert(viewportManager_ && "비었습니다. 초기화 실패");
    if (!viewportManager_) { return false; }

    return true;
}

void ObjectSelectionController::pickObjectAt(PickContext context) 
{
    if (!context.candidates) { return; }

    Math::Ray ray = calculateRay(context.pos.x, context.pos.y);

    shared_ptr<SceneObject> picked = nullptr;
    float minDist = numeric_limits<float>::max();

    for (const auto& o : *context.candidates)
    {
        if (!o) { continue; }

        float dist = 0.0f;
        if (o->intersects(ray.origin, ray.direction, dist)) 
        {
            if (dist < minDist) 
            {
                minDist = dist;
                picked = o;
            }
        }
    }
    EditorEventPublisher::get().publish(SelectionRequestedEvent{ picked, context.isShift, context.isAlt });
}

void ObjectSelectionController::pickArea(PickContext context) 
{
    if (!context.candidates) { return; }

    vector<shared_ptr<SceneObject>> selected;
    auto& activeCam = viewportManager_->getActiveCam();

    XMMATRIX view = activeCam.getViewMatrixXM();
    XMMATRIX proj = activeCam.getProjectionMatrixXM();
    float width = activeCam.getWidth();
    float height = activeCam.getHeight();

    //드래그 영역 계산
    long left = min(context.dragStartPos.x, context.pos.x);
    long right = max(context.dragStartPos.x, context.pos.x);
    long top = min(context.dragStartPos.y, context.pos.y);
    long bot = max(context.dragStartPos.y, context.pos.y);

    for (const auto& o : *context.candidates) 
    {
        if (!o) { continue; }

        XMVECTOR worldPos = XMLoadFloat3(&o->getPosition());
        XMVECTOR screenVec = XMVector3Project(worldPos, 0, 0, width, height, 0, 1, proj, view, XMMatrixIdentity());
        XMFLOAT3 screenPos;
        XMStoreFloat3(&screenPos, screenVec);

        if (screenPos.z < 1.0f && screenPos.x >= left && screenPos.x <= right &&
            screenPos.y >= top && screenPos.y <= bot) 
        {
            selected.push_back(o);
        }
    }

    if (!selected.empty()) 
    {
        vector<shared_ptr<Selection::Selectable>> converted(selected.begin(), selected.end());
        EditorEventPublisher::get().publish(SelectionDragRequestedEvent{converted, context.isShift, context.isAlt});
    }
}

Math::Ray ObjectSelectionController::calculateRay(int screenX, int screenY) const
{
    Math::Ray ray;
    auto& activeCam = viewportManager_->getActiveCam();

    float width = activeCam.getWidth();
    float height = activeCam.getHeight();
    XMMATRIX view = activeCam.getViewMatrixXM();
    XMMATRIX proj = activeCam.getProjectionMatrixXM();
    XMMATRIX world = XMMatrixIdentity();

    XMVECTOR mouseNear = XMVectorSet((float)screenX, (float)screenY, 0.0f, 1.0f);
    XMVECTOR mouseFar = XMVectorSet((float)screenX, (float)screenY, 1.0f, 1.0f);

    XMVECTOR rayNear = XMVector3Unproject(mouseNear, 0, 0, width, height, 0.0f, 1.0f, proj, view, world);
    XMVECTOR rayFar = XMVector3Unproject(mouseFar, 0, 0, width, height, 0.0f, 1.0f, proj, view, world);
    XMVECTOR rayDir = XMVector3Normalize(rayFar - rayNear);

    XMStoreFloat3(&ray.origin, rayNear);
    XMStoreFloat3(&ray.direction, rayDir);

    return ray;
}
