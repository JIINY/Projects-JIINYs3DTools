#pragma once
#include <memory>
#include <d3d11.h>


namespace Render 
{
	class ConstantBuffer;
		
	enum class ShaderStage //HLSL의 레지스터 슬롯(b0, b1, b2...)에 대응하는 개념
	{
		Vertex,
		Pixel
	};


	class IShader 
	{
	public:
		virtual ~IShader() = default;

		virtual void bind(ID3D11DeviceContext* context) = 0;
		virtual void unbind(ID3D11DeviceContext* context) = 0;

		virtual void setConstantBuffer(uint32_t slot, ShaderStage stage, std::shared_ptr<ConstantBuffer> buffer, ID3D11DeviceContext* context) = 0;
	};
}
