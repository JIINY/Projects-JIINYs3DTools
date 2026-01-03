#include "BaseObjectManager.hpp"
#include <cassert>
#include <vector>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include "renderer/Renderer.hpp"
#include "render/RenderObject.hpp"
#include "render/PixelShader.hpp"
#include "render/primitives/Cube.hpp"

#include "../resources/MaterialManager.hpp"
#include "../shaders/Lit_BlinnPhong/BlinnPhongMaterial.hpp"
using namespace std;
using namespace Render;


BaseObjectManager* BaseObjectManager::get() 
{
	static BaseObjectManager instance;
	return &instance;
}


void BaseObjectManager::initialize(Renderer* renderer) 
{
	assert(renderer && "[에러] BaseObjectManager 초기화 실패: 렌더러가 null입니다.");
	if (isInitialized_) { return; }

	device_ = renderer->getDevice();
	assert(device_ && "[에러] BaseObjectManager 초기화 실패: 디바이스를 가져올 수 없습니다.");
	context_ = renderer->getDeviceContext();
	assert(context_ && "[에러] BaseObjectManager 초기화 실패: 컨텍스트를 가져올 수 없습니다.");

	isInitialized_ = true;
}

void BaseObjectManager::draw(Renderer* renderer, const DirectX::XMMATRIX& viewMat, const DirectX::XMMATRIX& projMat, shared_ptr<Render::PixelShader> overridePS)
{
	assert(isInitialized_ && "[에러] BaseObjectManager가 초기화되지 않았습니다.");
	assert(renderer && "[에러] draw호출시 렌더러가 null입니다.");

	//TODO: 여기에 Label필터링 로직 추가

	Math::Mat4 view, proj;
	DirectX::XMStoreFloat4x4(&view, viewMat);
	DirectX::XMStoreFloat4x4(&proj, projMat);

	//Static
	for (const auto& obj : staticObjects_)
	{
		if (obj->isVisible()) { obj->draw(context_, view, proj, overridePS); }
	}

	for (const auto& obj : dynamicObjects_)
	{
		if (obj->isVisible()) { obj->draw(context_, view, proj, overridePS); }
	}
}

std::shared_ptr<RenderObject> BaseObjectManager::createCube(float size) 
{
	assert(isInitialized_ && "[에러] BaseObjectManager가 초기화되지 않았습니다.");

	auto newObj = make_shared<RenderObject>();
	auto mesh = make_shared<Render::Primitives::Cube>(size);
	mesh->initialize(device_);

	auto material = MaterialManager::get()->createMaterial<Render::Materials::BlinnPhongMaterial>(
		//L"shaders/Lit_BlinnPhong/Lit_VS_BlinnPhong.hlsl",
		//L"shaders/Lit_BlinnPhong/Lit_PS_BlinnPhong.hlsl"
	);

	newObj->initialize(device_, mesh, material);
	newObj->setDynamic(false);
	staticObjects_.push_back(newObj);

	return newObj;
}

void BaseObjectManager::setDynamicState(std::shared_ptr<RenderObject> obj, bool makeDynamic) 
{
	if (!obj || obj->isDynamic() == makeDynamic) { return; }

	auto& sourceList = makeDynamic ? staticObjects_ : dynamicObjects_;
	auto it = remove(sourceList.begin(), sourceList.end(), obj);
	if (it != sourceList.end()) { sourceList.erase(it, sourceList.end()); }

	obj->setDynamic(makeDynamic);
	auto& targetList = makeDynamic ? dynamicObjects_ : staticObjects_;
	targetList.push_back(obj);
}


void BaseObjectManager::shutdown() 
{
	staticObjects_.clear(); //shared_ptr라서 vector를 클리어하면 메모리 자동 해제
	dynamicObjects_.clear();

	device_ = nullptr;
	context_ = nullptr;
	isInitialized_ = false;
}
