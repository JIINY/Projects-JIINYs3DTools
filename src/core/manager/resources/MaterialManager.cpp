#include "MaterialManager.hpp"
#include <cassert>
#include "ShaderManager.hpp"
using namespace std;


MaterialManager* MaterialManager::get() 
{
	static MaterialManager instance;
	return &instance;
}

void MaterialManager::initialize(ID3D11Device* device, ShaderManager* shaderManager) 
{
	assert(shaderManager && "[에러] MaterialManager 초기화 실패: device 또는 ShaderManager가 null입니다.");
	if (isInitialized_) { return; }

	device_ = device;
	shaderManager_ = shaderManager;
	isInitialized_ = true;
}

void MaterialManager::shutdown() 
{
	shaderManager_ = nullptr;
	device_ = nullptr;
	isInitialized_ = false;
}
