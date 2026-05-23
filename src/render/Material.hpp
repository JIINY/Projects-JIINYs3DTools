#pragma once
#include "IMaterial.hpp"
#include <memory>
#include <d3d11.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <DirectXMath.h>
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "ConstantBuffer.hpp"
#include "common/Math.hpp"


namespace Render 
{
	struct MaterialPropData
	{
		size_t offset;
		size_t size;
	};


	class Material : public IMaterial
	{
	public:
		virtual ~Material() = default;

		virtual void initialize(ID3D11Device* device);
		void bind(ID3D11DeviceContext* context) override;

		//로드 실패시 비상용 형광연두(VS)/핑크(PS) 연결
		void loadVertexShader(ID3D11Device* device, std::wstring_view filepath, std::string_view entryPoint = "vsMain");
		void loadPixelShader(ID3D11Device* device, std::wstring_view filepath, std::string_view entryPoint = "psMain");

		//수동 교체
		void setVertexShader(std::shared_ptr<VertexShader> vs) { vertexShader_ = vs; }
		void setPixelShader(std::shared_ptr<PixelShader> ps) { pixelShader_ = ps; }
		std::shared_ptr<VertexShader> getVertexShader() { return vertexShader_; }
		std::shared_ptr<PixelShader> getPixelShader() { return pixelShader_; }
		const std::wstring& getVSPath() const { return vsPath_; }
		const std::wstring& getPSPath() const { return psPath_; }
		const std::string& getVSEntry() const { return vsEntry_; }
		const std::string& getPSEntry() const { return psEntry_; }

		std::shared_ptr<ConstantBuffer> getConstantBuffer() { return constantBuffer_; }
		const std::vector<uint8_t>& getBufferData() const { return bufferData_; }

		static std::shared_ptr<PixelShader> getWireBlackPS() { return wireBlack_; }
		static std::shared_ptr<PixelShader> getWireRedPS() { return wireRed_; }
		static std::shared_ptr<PixelShader> getDefaultPS() { return default_; }
		static bool prepareDefaultShaders(ID3D11Device* device);


		template <typename T>
		void addProperty(const std::string& name, const T& defaultValue)
		{
			if (this->propertyMap_.find(name) != this->propertyMap_.end()) { return; }

			MaterialPropData data;
			data.offset = this->bufferData_.size();
			data.size = sizeof(T);

			this->propertyMap_[name] = data;

			size_t oldSize = this->bufferData_.size();
			this->bufferData_.resize(oldSize + sizeof(T));
			memcpy(this->bufferData_.data() + oldSize, &defaultValue, sizeof(T));

			this->isDirty_ = true;
		}

		template <typename T>
		void setProperty(const std::string& name, const T& value)
		{
			auto it = this->propertyMap_.find(name);
			if (it != this->propertyMap_.end())
			{
				if (it->second.size != sizeof(T)) { return; }

				uint8_t* dest = &this->bufferData_[it->second.offset];
				memcpy(dest, &value, sizeof(T));
			}
			else
			{
				addProperty(name, value);
			}
		}

		void setColor(const std::string& name, const Math::Vec4& color)
		{
			setProperty(name, DirectX::XMFLOAT4(color.x, color.y, color.z, color.w));
		}

		void createBuffer(ID3D11Device* device);


	protected:
		std::shared_ptr<VertexShader> vertexShader_;
		std::shared_ptr<PixelShader> pixelShader_;
		std::wstring vsPath_ = L"";
		std::wstring psPath_ = L"";
		std::string vsEntry_ = "vsMain";
		std::string psEntry_ = "psMain";
		std::shared_ptr<ConstantBuffer> constantBuffer_;
		std::vector<uint8_t> bufferData_;
		std::unordered_map<std::string, MaterialPropData> propertyMap_;
		bool isDirty_ = false;


	private:
		static std::shared_ptr<VertexShader> fallbackVS_;
		static std::shared_ptr<PixelShader> errorPS_Pink_;
		static std::shared_ptr<PixelShader> errorVS_Green_;
		static std::shared_ptr<PixelShader> wireBlack_;
		static std::shared_ptr<PixelShader> wireRed_;
		static std::shared_ptr<PixelShader> default_;
	};
}
