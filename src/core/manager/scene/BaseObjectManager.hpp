#pragma once
#include <vector>
#include <memory>
#include <DirectXMath.h>
#include <d3d11.h>
#include <algorithm>

namespace Render 
{ 
	class RenderObject;
	class PixelShader;
}
class Renderer;


class BaseObjectManager 
{
public:
	static BaseObjectManager* get();

	void initialize(Renderer* renderer);
	void draw(Renderer* renderer, const DirectX::XMMATRIX& viewMat, const DirectX::XMMATRIX& projMat, std::shared_ptr<Render::PixelShader> overridePS = nullptr);
	void shutdown();

	std::shared_ptr<Render::RenderObject> createCube(float size);
	void setDynamicState(std::shared_ptr<Render::RenderObject> obj, bool makeDynamic);

	//TODO: ObjectLabel 필터링용 함수 추가
	//void setLabelVisibility(ObjectLabel label, bool isVisible);

private:
	BaseObjectManager() = default;
	~BaseObjectManager() = default;
	BaseObjectManager(const BaseObjectManager&) = delete;
	BaseObjectManager& operator=(const BaseObjectManager&) = delete;

	bool isInitialized_ = false;
	ID3D11Device* device_ = nullptr;
	ID3D11DeviceContext* context_ = nullptr;

	std::vector<std::shared_ptr<Render::RenderObject>> staticObjects_;
	std::vector<std::shared_ptr<Render::RenderObject>> dynamicObjects_;

	//TODO: Label 필터링 마스크
	//uint32_t labelVisibilityMask_ = 0xFFFFFFFF;
};
