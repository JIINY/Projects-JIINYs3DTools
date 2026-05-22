#pragma once
#include <memory>
#include <vector>
#include "common/Math.hpp"
#include "common/Mode.hpp"
#include "Gizmo.hpp"
#include "event/editorEvent/EditorEventSubscriber.hpp"
#include "event/editorEvent/selection/SelectionEvent.hpp"
#include "imgui.h"

namespace Render { class RenderCommandQueue; }
class SceneObject;


class GizmoController
{
public:
    GizmoController();
    ~GizmoController();

    bool initialize(ID3D11Device* device);
    void update(float deltaTime, const Math::Ray& mouseRay);

    void addToRenderQueue(Render::RenderCommandQueue* queue, const DirectX::XMMATRIX& viewMat);
    void onSelectionChanged(const SelectionChangedEvent& event);
    void setMode(TransformMode mode);
    TransformMode getMode() { return gizmo_->getMode(); }
    Math::Axis getDragAxis() const { return dragAxis_; }


private:
    std::vector<EditorEventSubscriptionID> editorEventSubID_;
    std::unique_ptr<Render::Tools::Gizmo> gizmo_;
    std::vector<std::shared_ptr<SceneObject>> targets_;
    Math::Mat4 gizmoWorldMat_;

    bool isDragging_ = false;
    Math::Axis dragAxis_ = Math::Axis::Count;
    Math::Mat4 dragStartGizmoWorldMat_ = {};
    float prevProjection_ = 0.0f;
    Math::Vec3 rotatePlaneNormal_ = { 0.0f, 0.0f, 0.0f };
    Math::Vec3 rotatePlaneCenter_ = { 0.0f, 0.0f, 0.0f };
    Math::Vec3 rotateStartVec_ = { 0.0f, 0.0f, 0.0f }; //드래그 시작지점의 (P0 - center) 벡터, 정규화됨
    float rotateMinStartLength_ = 0.1f;
    float rotatePrevDistance_ = 0.0f;
    const float sensitivity_ = 0.5f;

    Math::Vec3 calculatePivot() const;
    void initializeGizmoTransform();
    float projectRayOnAxis(const Math::Ray& mouseRay, Math::Axis axis) const;

    float getMouseDelta(const Math::Ray& mouseRay);
    void onMouseDown(const Math::Ray& mouseRay);
    void onMouseHold(const MouseHoldEditorEvent& event);
    void onMouseUp();
};
