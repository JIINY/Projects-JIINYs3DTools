#include "MaterialManager.hpp"
#include <cassert>
#include "../shaders/ShaderInclude.hpp"
#include "ShaderManager.hpp"
#include "render/Material.hpp"
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

shared_ptr<Render::Material> MaterialManager::createMaterial(const wstring& preset)
{
	auto it = presetMap.find(preset);
	if (it == presetMap.end())
	{
		assert(0 && "presetMap에 없음");
		return nullptr;
	}

	auto newMat = std::make_shared<Render::Material>();
	newMat->initialize(device_);
	
	it->second(*newMat); //프리셋 함수 적용
	newMat->setPreset(preset);

	auto vs = shaderManager_->getVertexShader(newMat->getVSPath(), newMat->getVSEntry());
	auto ps = shaderManager_->getPixelShader(newMat->getPSPath(), newMat->getPSEntry());

	if (vs) { newMat->setVertexShader(vs); } //fallback처리를 위한 방어코드
	if (ps) { newMat->setPixelShader(ps); }

	if (!newMat->getBufferData().empty())
	{
		newMat->createBuffer(device_);
	}

	return newMat;
}
