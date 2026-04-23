#pragma once
#include <vector>
#include <d3d11.h>
#include <wrl/client.h>


namespace Render 
{
	class ConstantBuffer 
	{
	public:
		virtual ~ConstantBuffer() = default;

		//GPU에 버퍼공간을 만듬, T: 구조체 타입(e.g. LightBuffer, MatrixBuffer)
		template <typename T>
		bool initialize(ID3D11Device* device) 
		{
			return initializeRaw(device, sizeof(T));
		}

		bool initializeRaw(ID3D11Device* device, UINT byteWidth);
		void updateRaw(ID3D11DeviceContext* context, const void* data, UINT byteWidth);

		//업데이트: CPU의 데이터를 GPU로 전송
		template <typename T>
		void update(ID3D11DeviceContext* context, const T& data) 
		{
			updateRaw(context, &data, sizeof(T));
		}

		//쉐이더 클래스용
		ID3D11Buffer* getBuffer() const { return buffer_.Get(); }

		void bindVS(ID3D11DeviceContext* context, UINT slot) const;
		void bindPS(ID3D11DeviceContext* context, UINT slot) const;


	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_;
	};
}
