#include "BaseObjectManager.hpp"
#include <cassert>
#include <vector>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include <algorithm>
#include "renderer/Renderer.hpp"
#include "render/RenderCommandQueue.hpp"
#include "render/PixelShader.hpp"
#include "object/SceneObject.hpp"
#include "render/primitives/PrimitivesInclude.hpp"
#include "utils/NameGenerator.hpp"

#include "../resources/MaterialManager.hpp"
#include "../shaders/Lit_BlinnPhong/Lit_Preset_BlinnPhong.hpp"

#include "event/appEvent/AppEventPublisher.hpp"
#include "event/editorEvent/EditorEventPublisher.hpp"
#include "event/appEvent/state/SceneObjectChangedEvent.hpp"

#include "common/DebugLog.hpp"
using namespace std;
using namespace Render;
using namespace DirectX;


bool BaseObjectManager::initialize(Renderer* renderer, MaterialManager* matManager) 
{
	assert(renderer && "renderer가 없습니다.");
	if (!renderer) { return false; }
	assert(matManager && "materialManager가 없습니다.");
	if (!matManager) { return false; }

	renderer_ = renderer;
	materialManager_ = matManager;
	device_ = renderer->getDevice();
	assert(device_ && "device를 가져올 수 없습니다.");
	if (!device_) { return false; }
	context_ = renderer->getDeviceContext();
	assert(context_ && "context를 가져올 수 없습니다.");
	if (!context_) { return false; }

	return true;
}

shared_ptr<SceneObject> BaseObjectManager::createPrimitive(const Primitives::PrimitiveData& data)
{
	string name;
	ColliderType colType = ColliderType::Primitive;
	Primitives::PrimitiveType pType = Primitives::PrimitiveType::Count;
	shared_ptr<Mesh> mesh = nullptr;

	switch (data.type)
	{
	case Primitives::PrimitiveType::Cube: 
	{
		name = NameGenerator::generate("Box");
		pType = Primitives::PrimitiveType::Cube;
		mesh = make_shared<Primitives::Cube>(data.width, data.height, data.depth, data.segmentsX, data.segmentsY, data.segmentsZ);
		break;
	}
	case Primitives::PrimitiveType::Cylinder: 
	{
		name = NameGenerator::generate("Cylinder");
		pType = Primitives::PrimitiveType::Cylinder;
		mesh = make_shared<Primitives::Cylinder>(data.radiusTop, data.radiusBot, data.height, data.segmentsX, data.segmentsY);
		break;
	}
	case Primitives::PrimitiveType::Capsule: 
	{
		name = NameGenerator::generate("Capsule");
		pType = Primitives::PrimitiveType::Capsule;
		mesh = make_shared<Primitives::Capsule>(data.radius, data.height, data.heightHemi, data.segmentsX, data.segmentsY, data.segmentsHemi);
		break;
	}
	case Primitives::PrimitiveType::Plane: 
	{
		name = NameGenerator::generate("Plane");
		pType = Primitives::PrimitiveType::Plane;
		mesh = make_shared<Primitives::Plane>(data.width, data.depth, data.segmentsX, data.segmentsY);
		break;
	}
	case Primitives::PrimitiveType::Sphere: 
	{
		name = NameGenerator::generate("Sphere");
		pType = Primitives::PrimitiveType::Sphere;
		mesh = make_shared<Primitives::Sphere>(data.radius, data.segmentsX, data.segmentsY);
		break;
	}
	case Primitives::PrimitiveType::Torus: 
	{
		name = NameGenerator::generate("Ring");
		pType = Primitives::PrimitiveType::Torus;
		mesh = make_shared<Primitives::Torus>(data.radius, data.thickness, data.segmentsX, data.segmentsY);
		break;
	}
	default:
	{
		assert(0 && "정의되지 않은 PrimitiveType이 생성요청 들어왔습니다.");
		break;
	}
	}

	if (mesh) 
	{
		mesh->initialize(device_);
	}

	auto material = materialManager_->createMaterial(L"Lit_BlinnPhong");

	auto newObj = std::make_shared<SceneObject>(name, colType, SceneObjectType::BaseObject);
	newObj->setPrimitiveType(pType);
	newObj->setDynamic(false);
	newObj->initialize(device_, mesh, material);

	//BoundingBox 설정
	BoundingBox box;
	switch (data.type)
	{
	case Primitives::PrimitiveType::Cube:
		box.Center = XMFLOAT3(0.0f, data.height * 0.5f, 0.0f);
		box.Extents = XMFLOAT3(data.width * 0.5f, data.height * 0.5f, data.depth * 0.5f);
		break;
	case Primitives::PrimitiveType::Cylinder:
		box.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
		box.Extents = XMFLOAT3(data.radiusTop, data.height * 0.5f, data.radiusTop);
		break;
	case Primitives::PrimitiveType::Sphere:
		box.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
		box.Extents = XMFLOAT3(data.radius, data.radius, data.radius);
		break;
	case Primitives::PrimitiveType::Plane:
		box.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
		box.Extents = XMFLOAT3(data.width * 0.5f, 0.01f, data.depth * 0.5f);
		break;
	case Primitives::PrimitiveType::Capsule:
		box.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
		box.Extents = XMFLOAT3(data.radius, data.height * 0.5f, data.radius);
		break;
	case Primitives::PrimitiveType::Torus:
	{
		box.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
		box.Extents = XMFLOAT3(data.radius + data.thickness, data.thickness, data.radius + data.thickness);

		//Torus는 Collider를 따로 처리
		newObj->setColliderType(ColliderType::Mesh);
		auto colMesh = make_shared<Primitives::Torus>(data.radius, data.thickness, data.segmentsX, max(data.segmentsY + 4, 8));
		auto colObj = make_shared<ColliderObject>(device_, colMesh);
		newObj->setColliderObject(colObj);

		break;
	}
	default:
		box.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
		box.Extents = XMFLOAT3(1.0f, 1.0f, 1.0f);
		break;
	}
	newObj->setBoundingBox(box);

	staticObjects_.push_back(newObj);
	AppEventPublisher::get().publish(SceneObjectChangedEvent{});

	return newObj;
}

void BaseObjectManager::addObject(shared_ptr<SceneObject> obj) 
{
	if (!obj) { return; }

	for (const auto& existing : staticObjects_) 
	{
		if (existing == obj) { return; }
	}

	staticObjects_.push_back(obj);
	AppEventPublisher::get().publish(SceneObjectChangedEvent{});
}

void BaseObjectManager::removeObject(std::shared_ptr<SceneObject> obj) 
{
	if (!obj) { return; }

	if (obj->isSelected())
	{
		EditorEventPublisher::get().publish(SelectionRequestedEvent{ obj, false, true });
	}

	staticObjects_.erase(
		remove(staticObjects_.begin(), staticObjects_.end(), obj), staticObjects_.end()
	);
	AppEventPublisher::get().publish(SceneObjectChangedEvent{});
}

void BaseObjectManager::removeAllObjects()
{
	for (const auto& o : staticObjects_)
	{
		if (o && o->isSelected())
		{
			EditorEventPublisher::get().publish(SelectionRequestedEvent{ o, false, true });
		}
	}
	staticObjects_.clear();
	AppEventPublisher::get().publish(SceneObjectChangedEvent{});
}

void BaseObjectManager::setDynamicState(std::shared_ptr<SceneObject> obj, bool makeDynamic) 
{
	if (!obj || obj->isDynamic() == makeDynamic) { return; }

	auto& sourceList = makeDynamic ? staticObjects_ : dynamicObjects_;
	auto it = remove(sourceList.begin(), sourceList.end(), obj);
	if (it != sourceList.end()) { sourceList.erase(it, sourceList.end()); }

	obj->setDynamic(makeDynamic);
	auto& targetList = makeDynamic ? dynamicObjects_ : staticObjects_;
	targetList.push_back(obj);
	AppEventPublisher::get().publish(SceneObjectChangedEvent{});
}


void BaseObjectManager::shutdown() 
{
	staticObjects_.clear(); //shared_ptr라서 vector를 클리어하면 메모리 자동 해제
	dynamicObjects_.clear();

	device_ = nullptr;
	context_ = nullptr;
}

void BaseObjectManager::addToRenderQueue(RenderCommandQueue* queue, const XMMATRIX& viewMat, const OverridePSType& type)
{
	if (!queue) { return; }

	XMMATRIX invView = XMMatrixInverse(nullptr, viewMat);
	XMVECTOR camPos = invView.r[3];

	for (const shared_ptr<SceneObject>& o : staticObjects_)
	{
		if (!o || !o->isVisible()) { continue; }

		XMMATRIX world = XMLoadFloat4x4(&o->getWorldMatForShader());
		XMVECTOR oPos = world.r[3];

		XMVECTOR diff = XMVectorSubtract(oPos, camPos);
		float depth = XMVectorGetX(XMVector3Length(diff));

		switch (type)
		{
		case OverridePSType::None:
			queue->addCommand(o.get(), depth);
			break;
		case OverridePSType::Collider:
		{
			auto colliderObj = o->getColliderObject();
			if (colliderObj) { queue->addCommand(colliderObj.get(), depth, OverridePSType::Collider); }
			break;
		}
		case OverridePSType::Black:
		{
			if (o->isSelected()) { queue->addCommand(o.get(), depth, OverridePSType::Red); }
			else { queue->addCommand(o.get(), depth, type); }
			break;
		}
		default:
			queue->addCommand(o.get(), depth);
			break;
		}
	}

	for (const shared_ptr<SceneObject>& o : dynamicObjects_)
	{
		if (!o || !o->isVisible()) { continue; }

		XMMATRIX world = XMLoadFloat4x4(&o->getWorldMatForShader());
		XMVECTOR oPos = world.r[3];

		XMVECTOR diff = XMVectorSubtract(oPos, camPos);
		float depth = XMVectorGetX(XMVector3Length(diff));

		switch (type)
		{
		case OverridePSType::None:
			queue->addCommand(o.get(), depth);
			break;
		case OverridePSType::Collider:
		{
			auto colliderObj = o->getColliderObject();
			if (colliderObj) { queue->addCommand(colliderObj.get(), depth, OverridePSType::Collider); }
			break;
		}
		case OverridePSType::Black:
		{
			if (o->isSelected()) { queue->addCommand(o.get(), depth, OverridePSType::Red); }
			else { queue->addCommand(o.get(), depth, type); }
			break;
		}
		default:
			queue->addCommand(o.get(), depth);
			break;
		}
	}
}

void BaseObjectManager::updateTransforms()
{
	for (auto& obj : staticObjects_)
	{
		if (obj) { obj->updateTransform(); }
	}
	for (auto& obj : dynamicObjects_)
	{
		if (obj) { obj->updateTransform(); }
	}
}
