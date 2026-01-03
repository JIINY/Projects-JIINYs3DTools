#pragma once
#include <string>
#include <memory>
#include <map>
#include <d3d11.h>
#include <wrl/client.h>

namespace Render {
	class VertexShader;
	class PixelShader;
}

class ShaderManager 
{
public:
	static ShaderManager* get();
	std::shared_ptr<Render::VertexShader> getVertexShader(const std::wstring& path, const std::string& entryPoint = "vsMain");
	std::shared_ptr<Render::PixelShader> getPixelShader(const std::wstring& path, const std::string& entryPoint = "psMain");

	void initialize(ID3D11Device* device);
	void shutdown();


private:
	ShaderManager() = default;
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;

	bool isInitialized_ = false;
	ID3D11Device* device_ = nullptr;

	std::map<std::wstring, std::shared_ptr<Render::VertexShader>> vsCache_;
	std::map<std::wstring, std::shared_ptr<Render::PixelShader>> psCache_;
};
