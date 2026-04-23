#include "ResourceCoordinator.hpp"
#include <memory>
#include <cassert>
#include <d3d11.h>
#include "core/manager/resources/ShaderManager.hpp"
#include "core/manager/resources/MaterialManager.hpp"
using namespace std;


ResourceCoordinator::ResourceCoordinator() : shaderManager_(make_unique<ShaderManager>()), materialManager_(make_unique<MaterialManager>()) {}
ResourceCoordinator::~ResourceCoordinator() = default;

bool ResourceCoordinator::initialize(ID3D11Device* device, ID3D11DeviceContext* context) 
{
	assert(device && context && "ResourceCoordinator 초기화 실패: 디바이스 또는 컨텍스트가 null입니다.");
	if (isInitialized_) { return false; }

	if (!shaderManager_->initialize(device)) { return false; }
	if (!materialManager_->initialize(device, shaderManager_.get())) { return false; }

	return true;
}

void ResourceCoordinator::shutdown() 
{
	if (!isInitialized_) { return; }

	materialManager_->shutdown(); //의존성 역순
	shaderManager_->shutdown();

	materialManager_ = nullptr;
	shaderManager_ = nullptr;
	isInitialized_ = false;
}
