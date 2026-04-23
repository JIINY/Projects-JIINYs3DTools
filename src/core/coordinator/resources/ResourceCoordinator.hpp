#pragma once
#include <memory>
#include <d3d11.h>

class ShaderManager;
class MaterialManager;


class ResourceCoordinator 
{
public:
	ResourceCoordinator();
	~ResourceCoordinator();

	ShaderManager* getShaderManager() const { return shaderManager_.get(); }
	MaterialManager* getMaterialManager() const { return materialManager_.get(); }

	bool initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	void shutdown();


private:
	std::unique_ptr<ShaderManager> shaderManager_;
	std::unique_ptr<MaterialManager> materialManager_;
	bool isInitialized_ = false;
};
