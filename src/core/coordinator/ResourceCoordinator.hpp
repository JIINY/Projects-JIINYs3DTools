#pragma once
#include <d3d11.h>

class ShaderManager;
class MaterialManager;


class ResourceCoordinator 
{
public:
	ShaderManager* getShaderManager() const { return shaderManager_; }
	MaterialManager* getMaterialManager() const { return materialManager_; }

	bool initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	void shutdown();


private:
	bool isInitialized_ = false;
	ShaderManager* shaderManager_ = nullptr;
	MaterialManager* materialManager_ = nullptr;
};
