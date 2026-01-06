#pragma once
#include <string>
#include <memory>
#include <d3d11.h>
#include <cassert>
#include "ShaderManager.hpp"
#include "render/Material.hpp"


class MaterialManager 
{
public:
	static MaterialManager* get();
	void initialize(ID3D11Device* device, ShaderManager* shaderManager);
	void shutdown();

	template <typename T>
	std::shared_ptr<T> createMaterial(const std::wstring& vsPath, const std::wstring& psPath) 
	{
		assert(isInitialized_ && "[에러] MaterialManager 초기화 안됨");

		auto newMat = std::make_shared<T>();
		if (!newMat->initialize(device_, vsPath, psPath)) //T클래스는 반드시 초기화 함수를 가져야 함
		{
			assert(0 && "[에러] 매티리얼 초기화 실패");
		}

		//매티리얼이 개별적으로 로드한 쉐이더 인스턴스 대신, 매니저가 관리하는 공유 인스턴스를 사용
		auto cachedVS = shaderManager_->getVertexShader(vsPath, "vsMain");
		auto cachedPS = shaderManager_->getPixelShader(psPath, "psMain");

		if (cachedVS) { newMat->setVertexShader(cachedVS); }
		if (cachedPS) { newMat->setPixelShader(cachedPS); }

		return newMat;
	}

	template <typename T>
	std::shared_ptr<T> createMaterial() 
	{
		assert(isInitialized_ && "[에러] MaterialManager 초기화 안됨");

		auto newMat = std::make_shared<T>();
		newMat->initialize(device_);

		return newMat;
	}

private:
	MaterialManager() = default;
	MaterialManager(const MaterialManager&) = delete;
	MaterialManager& operator=(const MaterialManager&) = delete;

	bool isInitialized_ = false;
	ID3D11Device* device_ = nullptr;
	ShaderManager* shaderManager_ = nullptr;
};
