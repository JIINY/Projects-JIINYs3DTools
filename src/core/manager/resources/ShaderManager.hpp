#pragma once
#include <string>
#include <memory>
#include <map>
#include <d3d11.h>
#include <wrl/client.h>

namespace Render 
{
	class VertexShader;
	class PixelShader;
}

class ShaderManager 
{
public:
	ShaderManager() = default;
	~ShaderManager() = default;

	std::shared_ptr<Render::VertexShader> getVertexShader(const std::wstring& path, const std::string& entryPoint = "vsMain");
	std::shared_ptr<Render::PixelShader> getPixelShader(const std::wstring& path, const std::string& entryPoint = "psMain");

	bool initialize(ID3D11Device* device);
	void shutdown();


private:
	std::map<std::wstring, std::shared_ptr<Render::VertexShader>> vsCache_;
	std::map<std::wstring, std::shared_ptr<Render::PixelShader>> psCache_;
	ID3D11Device* device_ = nullptr;
};
