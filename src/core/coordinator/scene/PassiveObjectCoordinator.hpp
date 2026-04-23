#pragma once
#include <memory>
#include <DirectXMath.h>
#include "render/primitives/PrimitivesType.hpp"
#include "render/RenderCommandQueue.hpp"

class Renderer;
class SceneObject;
class MaterialManager;
class BaseObjectManager;
namespace Render 
{ 
    class PixelShader;
}


class PassiveObjectCoordinator 
{
public:
    PassiveObjectCoordinator();
    ~PassiveObjectCoordinator();

    bool initialize(Renderer* renderer, MaterialManager* matManager);
    void update(float deltaTime);

    void addToRenderQueue(Render::RenderCommandQueue* queue, const DirectX::XMMATRIX& viewMat, const Render::OverridePSType& type);

    BaseObjectManager* getBaseObjectManager() const { return baseManager_.get(); }
    std::vector<std::shared_ptr<SceneObject>> getBaseObjects() const;

    std::shared_ptr<SceneObject> createPrimitive(const Render::Primitives::PrimitiveData& data);
    void addObject(std::shared_ptr<SceneObject> obj);
    void removeObject(std::shared_ptr<SceneObject> obj);
    void updateTransforms();

private:
    std::unique_ptr<BaseObjectManager> baseManager_ = nullptr;
};
