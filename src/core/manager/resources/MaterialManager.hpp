#pragma once
#include <string>
#include <memory>
#include <d3d11.h>
#include <cassert>
#include "render/Material.hpp"

class ShaderManager;


class MaterialManager 
{
public:
	bool initialize(ID3D11Device* device, ShaderManager* shaderManager);
	void shutdown();

	std::shared_ptr<Render::Material> createMaterial(const std::wstring& preset);


private:
	ID3D11Device* device_ = nullptr;
	ShaderManager* shaderManager_ = nullptr;
};
