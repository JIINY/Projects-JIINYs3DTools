#pragma once
#include <d3d11.h>
#include <memory>
#include <wrl/client.h>
#include <DirectXMath.h>
#include "../Renderer.hpp"
#include "render/ConstantBuffer.hpp"


class DX11Renderer : public Renderer {
public:
	bool initialize(void* hwnd) override;
	void beginFrame(const float* bgColor) override;
	void endFrame() override;
	void shutdown() override;

	ID3D11Device* getDevice() override { return pd3dDevice_.Get(); } //ComPtr::Get()으로 숨겨진 포인터 반환
	ID3D11DeviceContext* getDeviceContext() override { return pd3dDeviceContext_.Get(); }

	virtual void updateSceneConstants(const Render::SceneConstantBufferData& data) override;
	virtual void updateLightConstants(const Render::LightBufferData& data) override;
	ID3D11DepthStencilView* getDepthStencilView() const { return depthStencilView_.Get(); }
	ID3D11DepthStencilState* getDepthStencilStateOff() const { return depthStencilStateOff_.Get(); }
	ID3D11DepthStencilState* getDepthStencilStateAlways() const { return depthStencilStateAlways_.Get(); }

	void setDepthEnabled(bool enable);
	void setWireframeMode(bool enable) override;

private:
	HWND hwnd_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device> pd3dDevice_; //ComPtr 스마트 포인터로 메모리 누수 안되도록 관리
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pd3dDeviceContext_;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mainRenderTargetView_;
	
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStateOpaque_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> solidState_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> wireframeState_ = nullptr;
	Microsoft::WRL::ComPtr <ID3D11Texture2D> depthStencilBuffer_ = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> depthStencilView_ = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilStateOn_ = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilStateOff_ = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilStateAlways_ = nullptr;

	std::shared_ptr<Render::ConstantBuffer> sceneBuffer_;
	std::shared_ptr<Render::ConstantBuffer> lightBuffer_;

	bool createDevice(HWND hwnd);
	void createRenderTarget();
};
