#pragma once
#include <vector>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include "common/Math.hpp"
#include "imgui.h"

struct GridChangedEvent;
namespace Render 
{ 
    class RenderCommandQueue;
    class RenderObject;
}
namespace Render::Tools { class Grid; }
class Renderer;
class WorldPivotUI;
class ViewportCameraManager;
class MaterialManager;
class GizmoController;


struct ToolObjectContext
{
    Renderer* renderer = nullptr;
    ViewportCameraManager* viewCamManager = nullptr;
    MaterialManager* matManager = nullptr;
};

class ToolObjectManager 
{
public:
    ToolObjectManager();
    ~ToolObjectManager();

    bool initialize(ToolObjectContext context);
    void drawImGui();
    void update(float deltaTime, const Math::Ray& mouseRay);

    void addRenderQueue(Render::RenderCommandQueue* queue, const DirectX::XMMATRIX& viewMat);
    void addGridToRenderQueue(Render::RenderCommandQueue* queue, const DirectX::XMMATRIX& viewMat);
    void addGizmoToRenderQueue(Render::RenderCommandQueue* queue, const DirectX::XMMATRIX& viewMat);
    Render::Tools::Grid* getGrid() const { return grid_.get(); }
    GizmoController* getGizmoController() const { return gizmoController_.get(); }


private:
    std::vector<std::shared_ptr<Render::RenderObject>> tools_;
    std::unique_ptr<Render::Tools::Grid> grid_;
    std::unique_ptr<WorldPivotUI> worldPivotUI_;
    std::unique_ptr<GizmoController> gizmoController_;

    bool showGrid_ = true;
    ID3D11Device* device_ = nullptr;
    ID3D11DeviceContext* context_ = nullptr;
    ViewportCameraManager* viewCamManager_ = nullptr;
    MaterialManager* materialManager_ = nullptr;

    void onGridChanged(const GridChangedEvent& event);
};
