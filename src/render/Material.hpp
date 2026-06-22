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
#include "ui/WidgetData.hpp"


namespace Render 
{
	struct MaterialPropData
	{
		size_t offset;
		size_t size;
	};

	struct UIPropData
	{
		UI::WidgetData data = UI::WidgetData::Count;
		UI::WidgetType widget = UI::WidgetType::Count;
		UI::WidgetSettings settings;
	};

	class Material : public IMaterial
	{
	public:
		virtual ~Material() = default;

		virtual void initialize(ID3D11Device* device);
		void bind(ID3D11DeviceContext* context) override;

		//수동 교체
		void setPreset(const std::wstring& preset) { preset_ = preset; }
		void setVertexShader(std::shared_ptr<VertexShader> vs) { vertexShader_ = vs; }
		void setPixelShader(std::shared_ptr<PixelShader> ps) { pixelShader_ = ps; }
		void setVSPath(const std::wstring& path) { vsPath_ = path; }
		void setPSPath(const std::wstring& path) { psPath_ = path; }

		const std::wstring& getPreset() const { return preset_; }
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
		static std::shared_ptr<PixelShader> getColliderPS() { return collider_; }
		static std::shared_ptr<VertexShader> getFallbackVS() { return fallbackVS_; }
		static std::shared_ptr<PixelShader> getErrorVS() { return errorVS_Green_; }
		static std::shared_ptr<PixelShader> getErrorPS() { return errorPS_Pink_; }
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

		template <typename T>
		T getProperty(const std::string& name) const
		{
			auto it = propertyMap_.find(name);
			if (it != propertyMap_.end() && it->second.size == sizeof(T))
			{
				T value;
				memcpy(&value, &bufferData_[it->second.offset], sizeof(T));
				return value;
			}
			return T{};
		}

		void setColor(const std::string& name, const Math::Vec4& color)
		{
			setProperty(name, DirectX::XMFLOAT4(color.x, color.y, color.z, color.w));
		}
		void useProperty(const std::string& name, UI::WidgetData dataType, UI::WidgetType widget, UI::WidgetSettings settings);
		const std::unordered_map<std::string, UIPropData>& getUIPropertyMap() const { return uiPropertyMap_; }
		const std::unordered_map<std::string, MaterialPropData>& getPropertyMap() const { return propertyMap_; }
		void createBuffer(ID3D11Device* device);


	protected:
		std::shared_ptr<VertexShader> vertexShader_;
		std::shared_ptr<PixelShader> pixelShader_;
		std::wstring preset_ = L"";
		std::wstring vsPath_ = L"";
		std::wstring psPath_ = L"";
		std::string vsEntry_ = "vsMain";
		std::string psEntry_ = "psMain";
		std::shared_ptr<ConstantBuffer> constantBuffer_;
		std::vector<uint8_t> bufferData_;
		std::unordered_map<std::string, MaterialPropData> propertyMap_;
		std::unordered_map<std::string, UIPropData> uiPropertyMap_;
		bool isDirty_ = false;


	private:
		static std::shared_ptr<VertexShader> fallbackVS_;
		static std::shared_ptr<PixelShader> errorPS_Pink_;
		static std::shared_ptr<PixelShader> errorVS_Green_;
		static std::shared_ptr<PixelShader> wireBlack_;
		static std::shared_ptr<PixelShader> wireRed_;
		static std::shared_ptr<PixelShader> collider_;
		static std::shared_ptr<PixelShader> default_;
	};
}
