#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>
#include <d3d11.h>
#include "render/primitives/PrimitivesType.hpp"
#include "render/RenderCommand.hpp"
#include "renderer/Renderer.hpp"

namespace Render 
{ 
	class RenderCommandQueue;
	class PixelShader; 
}
class SceneObject;
class MaterialManager;


class BaseObjectManager 
{
public:
	BaseObjectManager() = default;
	~BaseObjectManager() = default;

	bool initialize(Renderer* renderer, MaterialManager* matManager);
	void shutdown();

	void addToRenderQueue(Render::RenderCommandQueue* queue, const DirectX::XMMATRIX& viewMat, const Render::OverridePSType& type = Render::OverridePSType::None);
	std::shared_ptr<SceneObject> createPrimitive(const Render::Primitives::PrimitiveData& data);
	void setDynamicState(std::shared_ptr<SceneObject> obj, bool makeDynamic);

	void addObject(std::shared_ptr<SceneObject> obj);
	void removeObject(std::shared_ptr<SceneObject> obj);
	void updateTransforms();

	const std::vector<std::shared_ptr<SceneObject>>& getStaticObjects() const { return staticObjects_; }
	const std::vector<std::shared_ptr<SceneObject>>& getDynamicObjects() const { return dynamicObjects_; }


private:
	std::vector<std::shared_ptr<SceneObject>> staticObjects_;
	std::vector<std::shared_ptr<SceneObject>> dynamicObjects_;

	Renderer* renderer_ = nullptr;
	ID3D11Device* device_ = nullptr;
	ID3D11DeviceContext* context_ = nullptr;
	MaterialManager* materialManager_ = nullptr;
};
