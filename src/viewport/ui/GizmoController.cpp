#include "GizmoController.hpp"
#include <vector>
#include "common/Math.hpp"
#include "Gizmo.hpp"
#include "object/SceneObject.hpp"
#include "render/RenderCommandQueue.hpp"

#include "event/editorEvent/EditorEventPublisher.hpp"
#include "event/editorEvent/EditorEventSubscriber.hpp"
#include "event/editorEvent/io/MouseEvent.hpp"
#include "event/editorEvent/selection/SelectionEvent.hpp"
#include "event/editorEvent/transform/GizmoModeRequestedEvent.hpp"

#include "render/tools/GizmoObject.hpp"
#include "common/DebugLog.hpp"
using namespace std;
using namespace Math;
using namespace DirectX;


GizmoController::GizmoController() : gizmo_(make_unique<Render::Tools::Gizmo>())
{
    XMStoreFloat4x4(&gizmoWorldMat_, XMMatrixIdentity());
}
GizmoController::~GizmoController() = default;

bool GizmoController::initialize(ID3D11Device* device)
{
    if (!gizmo_) { return false; }

    auto gizmoID = EditorEventSubscriber::get().subscribe<SelectionChangedEvent>([this](const SelectionChangedEvent& event)
        {
            this->onSelectionChanged(event);
        });
    editorEventSubID_.push_back(gizmoID);

    auto gizmoMode = EditorEventSubscriber::get().subscribe<GizmoModeRequestedEvent>([this](const GizmoModeRequestedEvent& event)
        {
            this->setMode(event.mode);
        });
    editorEventSubID_.push_back(gizmoMode);

    auto mouseDown = EditorEventSubscriber::get().subscribe<MouseDownEditorEvent>([this](const MouseDownEditorEvent& event)
        {
            if (event.button_ != 0) { return; }
            this->onMouseDown(event.mouseRay_);
        });
    editorEventSubID_.push_back(mouseDown);

    auto mouseHold = EditorEventSubscriber::get().subscribe<MouseHoldEditorEvent>([this](const MouseHoldEditorEvent& event)
        {
            if (event.button_ != 0) { return; }
            this->onMouseHold(event);
        });
    editorEventSubID_.push_back(mouseHold);

    auto mouseUp = EditorEventSubscriber::get().subscribe<MouseUpEditorEvent>([this](const MouseUpEditorEvent& event)
        {
            if (event.button_ != 0) { return; }
            this->onMouseUp();
        });
    editorEventSubID_.push_back(mouseUp);

    return gizmo_->initialize(device);
}

void GizmoController::update(float deltaTime, const Math::Ray& mouseRay)
{
    if (targets_.empty()) { return; }

    Vec3 pivot = calculatePivot();
    XMMATRIX transMat = XMMatrixTranslation(pivot.x, pivot.y, pivot.z);
    XMStoreFloat4x4(&gizmoWorldMat_, transMat);

    gizmo_->update(mouseRay, transMat);
}

void GizmoController::addToRenderQueue(Render::RenderCommandQueue* queue, const XMMATRIX& viewMat)
{
    if (targets_.empty() || !gizmo_) { return; }

    gizmo_->addToRenderQueue(queue, viewMat);
}

void GizmoController::setMode(TransformMode mode)
{
    if (gizmo_) 
    {
        gizmo_->setMode(mode);
    }
}

void GizmoController::onSelectionChanged(const SelectionChangedEvent& event)
{
    targets_.clear();

    for (const auto& o : event.currentSelection)
    {
        auto sceneObj = std::dynamic_pointer_cast<SceneObject>(o);
        if (sceneObj)
        {
            targets_.push_back(sceneObj);
        }
    }

    if (!targets_.empty() && gizmo_)
    {
        initializeGizmoTransform();
    }
}

Vec3 GizmoController::calculatePivot() const
{
    if (targets_.empty()) 
    {
        return Vec3(0.0f, 0.0f, 0.0f);
    }

    XMVECTOR sumPos = XMVectorZero();
    int count = 0;

    for (const auto& o : targets_)
    {
        if (!o) { continue; }

        XMMATRIX world = XMLoadFloat4x4(&o->getWorldMatForShader());
        XMVECTOR scale, rot, pos;
        if (XMMatrixDecompose(&scale, &rot, &pos, world))
        {
            sumPos = XMVectorAdd(sumPos, pos);
            count++;
        }
    }

    if (count == 0)
    {
        return Vec3(0.0f, 0.0f, 0.0f);
    }

    sumPos = XMVectorScale(sumPos, 1.0f / (float)count);

    Vec3 pivot;
    XMStoreFloat3(&pivot, sumPos);
    return pivot;
}

void GizmoController::initializeGizmoTransform()
{
    Vec3 pivot = calculatePivot();
    XMMATRIX gizmoWorld = XMMatrixTranslation(pivot.x, pivot.y, pivot.z);
    XMStoreFloat4x4(&gizmoWorldMat_, gizmoWorld);

    gizmo_->getX()->setWorldMatrix(AxisInfo::GetRotationMX(Axis::X) * gizmoWorld, AxisInfo::GetRotationMX(Axis::X) * gizmoWorld);
    gizmo_->getY()->setWorldMatrix(AxisInfo::GetRotationMX(Axis::Y) * gizmoWorld, AxisInfo::GetRotationMX(Axis::Y) * gizmoWorld);
    gizmo_->getZ()->setWorldMatrix(AxisInfo::GetRotationMX(Axis::Z) * gizmoWorld, AxisInfo::GetRotationMX(Axis::Z) * gizmoWorld);
}

float GizmoController::getMouseDelta(const Ray& mouseRay)
{
    if (!isDragging_) { return 0.0f; }

    float currentProj = projectRayOnAxis(mouseRay, dragAxis_);
    float delta = currentProj - prevProjection_;
    prevProjection_ = currentProj;

    return delta;
}

void GizmoController::onMouseDown(const Ray& mouseRay)
{
    if (gizmo_->getHoveredAxis() == Axis::Count) { return; }

    isDragging_ = true;
    dragAxis_ = gizmo_->getHoveredAxis();
    dragStartGizmoWorldMat_ = gizmoWorldMat_;
    prevProjection_ = projectRayOnAxis(mouseRay, dragAxis_);

    GizmoDragEvent dragEv;
    dragEv.axis = dragAxis_;
    dragEv.delta = 0.0f;
    dragEv.mode = gizmo_->getMode();
    dragEv.state = GizmoDragState::Begin;
    EditorEventPublisher::get().publish(dragEv);
}

void GizmoController::onMouseHold(const MouseHoldEditorEvent& event)
{
    if (!isDragging_) { return; }

    GizmoDragEvent dragEv;
    dragEv.mode = gizmo_->getMode();
    
    float delta = 0.0f;
    if (dragEv.mode == TransformMode::Rotate)
    {
        switch (dragAxis_)
        {
        case Axis::X: delta = event.deltaY_ * sensitivity_; break;
        case Axis::Y: delta = event.deltaX_ * sensitivity_; break;
        case Axis::Z: delta = event.deltaX_ * sensitivity_; break;
        default: return;
        }
    }
    else
    {
        delta = getMouseDelta(event.mouseRay_);
    }

    if (fabs(delta) < 1e-6f) { return; }

    dragEv.axis = dragAxis_;
    dragEv.delta = delta;
    dragEv.state = GizmoDragState::Update;
    EditorEventPublisher::get().publish(dragEv);
}

void GizmoController::onMouseUp()
{
    if (!isDragging_) { return; }
    GizmoDragEvent dragEv;
    dragEv.axis = dragAxis_;
    dragEv.delta = 0.0f;
    dragEv.mode = gizmo_->getMode();
    dragEv.state = GizmoDragState::End;
    EditorEventPublisher::get().publish(dragEv);

    isDragging_ = false;
    dragAxis_ = Axis::Count;
    prevProjection_ = 0.0f;
}

float GizmoController::projectRayOnAxis(const Math::Ray& mouseRay, Axis axis) const
{
    XMVECTOR axisDir;
    switch (axis)
    {
    case Axis::X: axisDir = XMVectorSet(1, 0, 0, 0); break;
    case Axis::Y: axisDir = XMVectorSet(0, 1, 0, 0); break;
    case Axis::Z: axisDir = XMVectorSet(0, 0, 1, 0); break;
    default: return 0.0f;
    }

    XMVECTOR gizmoPos = XMVectorSet(dragStartGizmoWorldMat_._41, dragStartGizmoWorldMat_._42, dragStartGizmoWorldMat_._43, 0.0f);
    XMVECTOR rayOrigin = XMLoadFloat3(&mouseRay.origin);
    XMVECTOR rayDir = XMLoadFloat3(&mouseRay.direction);

    //마우스Ray와 축Ray의 최근접점 계산
    XMVECTOR w0 = XMVectorSubtract(rayOrigin, gizmoPos);
    float a = XMVectorGetX(XMVector3Dot(axisDir, axisDir));
    float b = XMVectorGetX(XMVector3Dot(axisDir, rayDir));
    float c = XMVectorGetX(XMVector3Dot(rayDir, rayDir));
    float d = XMVectorGetX(XMVector3Dot(axisDir, w0));
    float e = XMVectorGetX(XMVector3Dot(rayDir, w0));

    float denom = a * c - b * b;
    if (fabs(denom) < 1e-6f) { return prevProjection_; } //평행

    float s = (b * e - c * d) / denom; //축Ray 위의 최근접점 파라미터
    return s;
}
