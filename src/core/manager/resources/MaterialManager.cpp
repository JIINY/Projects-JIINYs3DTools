#include "MaterialManager.hpp"
#include <cassert>
#include "ShaderManager.hpp"
using namespace std;


bool MaterialManager::initialize(ID3D11Device* device, ShaderManager* shaderManager) 
{
	assert(device && "초기화 실패");
	assert(shaderManager && "초기화 실패");
	if (!device || !shaderManager) { return false; }

	device_ = device;
	shaderManager_ = shaderManager;
	return true;
}

void MaterialManager::shutdown() 
{
	device_ = nullptr;
	shaderManager_ = nullptr;
}
