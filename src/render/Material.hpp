#pragma once
#include "IMaterial.hpp"
#include <memory>
#include <d3d11.h>
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "ConstantBuffer.hpp"
#include "DirectXMath.h"


namespace Render 
{
	struct MaterialData 
	{
		DirectX::XMFLOAT4 color = {0.5f, 0.5f, 0.5f, 1.0f};
		float roughness = 0.0f;
		DirectX::XMFLOAT3 padding;
	};


	class Material : public IMaterial
	{
	public:
		Material();
		virtual ~Material();

		static bool prepareDefaultShaders(ID3D11Device* device);

		virtual void initialize(ID3D11Device* device);
		void bind(ID3D11DeviceContext* context) override;

		//로드 실패시 비상용 형광연두(VS)/핑크(PS) 연결
		void loadVertexShader(ID3D11Device* device, std::wstring_view filepath, std::string_view entryPoint = "vsMain");
		void loadPixelShader(ID3D11Device* device, std::wstring_view filepath, std::string_view entryPoint = "psMain");

		static std::shared_ptr<PixelShader> getWireframePS() { return wireframe_; }
		static std::shared_ptr<PixelShader> getDefaultPS() { return default_; }

		//수동 교체
		void setVertexShader(std::shared_ptr<VertexShader> vs) { vertexShader_ = vs; }
		void setPixelShader(std::shared_ptr<PixelShader> ps) { pixelShader_ = ps; }


	protected:
		std::shared_ptr<VertexShader> vertexShader_;
		std::shared_ptr<PixelShader> pixelShader_;
		std::shared_ptr<ConstantBuffer> constantBuffer_;

		template <typename T>
		void createBuffer(ID3D11Device* device) 
		{
			constantBuffer_ = std::make_shared<ConstantBuffer>();
			constantBuffer_->initialize<T>(device);
		}

		template <typename T>
		void updateData(ID3D11DeviceContext* context, const T& data) 
		{
			if (constantBuffer_) { constantBuffer_->update(context, data); }
		}


	private:
		static std::shared_ptr<VertexShader> fallbackVS_;
		static std::shared_ptr<PixelShader> errorPS_Pink_;
		static std::shared_ptr<PixelShader> errorVS_Green_;
		static std::shared_ptr<PixelShader> wireframe_;
		static std::shared_ptr<PixelShader> default_;
	};
}
