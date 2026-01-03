#include "ResourceCoordinator.hpp"
#include <cassert>
#include <d3d11.h>
#include "core/manager/resources/ShaderManager.hpp"
#include "core/manager/resources/MaterialManager.hpp"


bool ResourceCoordinator::initialize(ID3D11Device* device, ID3D11DeviceContext* context) 
{
	assert(device && context && "[에러] ResourceCoordinator 초기화 실패: 디바이스 또는 컨텍스트가 null입니다.");
	if (isInitialized_) { return false; }

	shaderManager_ = ShaderManager::get();
	materialManager_ = MaterialManager::get();

	shaderManager_->initialize(device);
	materialManager_->initialize(device, shaderManager_);

	isInitialized_ = true;
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
