#include "PassiveObjectCoordinator.hpp"
#include <DirectXMath.h>
#include "core/manager/resources/MaterialManager.hpp"
#include "core/manager/scene/BaseObjectManager.hpp"
#include "render/primitives/PrimitivesType.hpp"
#include "render/PixelShader.hpp"

#include "common/DebugLog.hpp"
using namespace std;
using namespace Render;
using namespace DirectX;


PassiveObjectCoordinator::PassiveObjectCoordinator() : baseManager_(make_unique<BaseObjectManager>()) {}
PassiveObjectCoordinator::~PassiveObjectCoordinator() = default;

bool PassiveObjectCoordinator::initialize(Renderer* renderer, MaterialManager* matManager) 
{
    if (!baseManager_->initialize(renderer, matManager)) { return false; }
    return true;
}

void PassiveObjectCoordinator::update(float deltaTime) 
{
    assert(baseManager_ && "update: baseObjManager가 초기화되지 않았습니다.");
}

shared_ptr<SceneObject> PassiveObjectCoordinator::createPrimitive(const Primitives::PrimitiveData& data)
{
    assert(baseManager_ && "createPrimitive: baseObjManager가 초기화되지 않았습니다.");
    return baseManager_->createPrimitive(data);
}

void PassiveObjectCoordinator::addObject(shared_ptr<SceneObject> obj)
{
    assert(baseManager_ && "addObject: baseObjManager가 초기화되지 않았습니다.");
    baseManager_->addObject(obj);
}

void PassiveObjectCoordinator::removeObject(shared_ptr<SceneObject> obj)
{
    assert(baseManager_ && "removeObject: baseObjManager가 초기화되지 않았습니다.");
    baseManager_->removeObject(obj);
}

vector<shared_ptr<SceneObject>> PassiveObjectCoordinator::getBaseObjects() const 
{
    vector<shared_ptr<SceneObject>> combinedList;

    if (!baseManager_) { return combinedList; }

    const auto& statics = baseManager_->getStaticObjects();
    const auto& dynamics = baseManager_->getDynamicObjects();

    combinedList.reserve(statics.size() + dynamics.size());
    combinedList.insert(combinedList.end(), statics.begin(), statics.end());
    combinedList.insert(combinedList.end(), dynamics.begin(), dynamics.end());

    return combinedList;
}

void PassiveObjectCoordinator::addToRenderQueue(RenderCommandQueue* queue, const XMMATRIX& viewMat, const OverridePSType& type)
{
    assert(baseManager_ && "update: baseObjManager가 초기화되지 않았습니다.");
    baseManager_->addToRenderQueue(queue, viewMat, type);
}

void PassiveObjectCoordinator::updateTransforms()
{
    baseManager_->updateTransforms();
}