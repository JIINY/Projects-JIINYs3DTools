#include "GizmoController.hpp"
#include <vector>
#include "common/Math.hpp"
#include "core/manager/resources/MaterialManager.hpp"
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

bool GizmoController::initialize(ID3D11Device* device, MaterialManager* matManager)
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

    return gizmo_->initialize(device, matManager);
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

    if (gizmo_->getMode() == TransformMode::Rotate)
    {
        XMMATRIX gizmoMat = XMLoadFloat4x4(&dragStartGizmoWorldMat_);
        Vec3 center;
        XMStoreFloat3(&center, gizmoMat.r[3]);

        Vec3 normal;
        switch (dragAxis_)
        {
        case Axis::X: normal = { 1.0f, 0.0f, 0.0f }; break;
        case Axis::Y: normal = { 0.0f, 1.0f, 0.0f }; break;
        case Axis::Z: normal = { 0.0f, 0.0f, 1.0f }; break;
        default: normal = { 0.0f, 1.0f, 0.0f }; break;
        }

        Vec3 hitPoint;
        if (castRayOnPlane(mouseRay, center, normal, hitPoint))
        {
            XMVECTOR centerV = XMLoadFloat3(&center);
            XMVECTOR normalV = XMLoadFloat3(&normal);
            XMVECTOR hitV = XMLoadFloat3(&hitPoint);

            //startVec = hitPoint - center, 법선 성분 제거
            XMVECTOR startV = XMVectorSubtract(hitV, centerV);
            XMVECTOR proj = XMVector3Dot(startV, normalV);
            startV = XMVectorSubtract(startV, XMVectorMultiply(proj, normalV));

            float startLen = XMVectorGetX(XMVector3Length(startV));
            if (startLen < rotateMinStartLength_)
            {
                rotateStartVec_ = { 0.0f, 0.0f, 0.0f };
            }
            else
            {
                startV = XMVector3Normalize(startV);
                XMStoreFloat3(&rotateStartVec_, startV);
            }
            rotatePlaneNormal_ = normal;
            rotatePlaneCenter_ = center;
            rotatePrevDistance_ = 0.0f;
        }
        else
        {
            //평면 교차 실패
            rotateStartVec_ = { 0.0f, 0.0f, 0.0f };
            rotatePlaneNormal_ = normal;
            rotatePlaneCenter_ = center;
            rotatePrevDistance_ = 0.0f;
        }
    }

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
        //onMouseDown에서 평면교차 실패시 회전 스킵
        XMVECTOR startV = XMLoadFloat3(&rotateStartVec_);
        float startLenSq = XMVectorGetX(XMVector3LengthSq(startV));
        if (startLenSq < 1e-8f) { return; }

        //평면 상태 로드
        XMVECTOR center = XMLoadFloat3(&rotatePlaneCenter_);
        XMVECTOR normal = XMLoadFloat3(&rotatePlaneNormal_);

        //현재 마우스ray를 평면과 교차
        Vec3 hitPoint;
        if (!castRayOnPlane(event.mouseRay_, rotatePlaneCenter_, rotatePlaneNormal_, hitPoint)) { return; }

        //현재 평면상 위치: hitPoint - center, 법선 성분 제거
        XMVECTOR hitV = XMLoadFloat3(&hitPoint);
        XMVECTOR currentV = XMVectorSubtract(hitV, center);

        XMVECTOR proj = XMVector3Dot(currentV, normal);
        currentV = XMVectorSubtract(currentV, XMVectorMultiply(proj, normal));

        //시작 직선 수직 방향: cross(normal, startV) - 평면 위에서 startV과 직교, 부호는 normal기준 오른손 법칙
        XMVECTOR perpDir = XMVector3Cross(normal, startV);
        float signedDist = XMVectorGetX(XMVector3Dot(currentV, perpDir));
        
        //signedDist는 시작 직선으로부터의 절대 거리(시작점 대비 누적량)
        //ActionHandler는 delta를 프레임 증분으로 누적하므로 이전 프레임 거리와의 차이를 발행 피룡
        float deltaDist = signedDist - rotatePrevDistance_;
        rotatePrevDistance_ = signedDist;

        float ringRadius = gizmo_->getX()->getData().radius_;
        delta = deltaDist / ringRadius * sensitivity_;
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

    //초기화
    isDragging_ = false;
    dragAxis_ = Axis::Count;
    prevProjection_ = 0.0f;

    rotatePlaneCenter_ = { 0.0f, 0.0f, 0.0f };
    rotatePlaneNormal_ = { 0.0f, 0.0f, 0.0f };
    rotateStartVec_ = { 0.0f, 0.0f, 0.0f };
    rotatePrevDistance_ = 0.0f;
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
    XMVECTOR w0 = XMVectorSubtract(gizmoPos, rayOrigin);
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
