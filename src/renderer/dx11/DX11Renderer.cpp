#pragma once
#include "DX11Renderer.hpp"
#include <cassert>
#include "render/lights/LightType.hpp"
#include "render/Mesh.hpp"
#include "render/Material.hpp"
using namespace std;


bool DX11Renderer::initialize(void* hwnd) 
{
	hwnd_ = static_cast<HWND>(hwnd);

	if (!createDevice(hwnd_)) { return false; }

	sceneBuffer_ = make_shared<Render::ConstantBuffer>();
	if (!sceneBuffer_->initialize<Render::SceneConstantBufferData>(pd3dDevice_.Get())) { return false; }

	lightBuffer_ = make_shared<Render::ConstantBuffer>();
	if (!lightBuffer_->initialize<Render::LightBufferData>(pd3dDevice_.Get())) { return false; }

	createRenderTarget();

	//깊이 버퍼를 위해 화면크기 가져오기
	RECT rc;
	GetClientRect(hwnd_, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;


	//깊이 버퍼 메모리 생성
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 24비트 깊이 + 8비트 스텐실
	depthBufferDesc.SampleDesc.Count = 1; // 멀티샘플링 안함
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // "이건 깊이 버퍼용이다"
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	HRESULT hr = pd3dDevice_->CreateTexture2D(&depthBufferDesc, nullptr, depthStencilBuffer_.GetAddressOf());
	assert(SUCCEEDED(hr) && "깊이 버퍼 메모리/텍스쳐 생성 실패");
	if (FAILED(hr)) { return false; }


	//Depth Stencil View 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	ZeroMemory(&depthViewDesc, sizeof(depthViewDesc));

	depthViewDesc.Format = depthBufferDesc.Format;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0;

	hr = pd3dDevice_->CreateDepthStencilView(depthStencilBuffer_.Get(), &depthViewDesc, depthStencilView_.GetAddressOf());
	assert(SUCCEEDED(hr) && "깊이 버퍼 생성 실패");
	if (FAILED(hr)) { return false; }


	//Depth Stencil State 생성
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));

	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS; //나보다 가까우면 그린다

	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = pd3dDevice_->CreateDepthStencilState(&dsDesc, depthStencilStateOn_.GetAddressOf());
	assert(SUCCEEDED(hr) && "깊이 버퍼 ON 설정 실패");
	if (FAILED(hr)) { return false; }

	dsDesc.DepthEnable = false;
	hr = pd3dDevice_->CreateDepthStencilState(&dsDesc, depthStencilStateOff_.GetAddressOf());
	assert(SUCCEEDED(hr) && "깊이 버퍼 OFF 설정 실패");
	if (FAILED(hr)) { return false; }


	//상시
	D3D11_DEPTH_STENCIL_DESC dsDescAlways;
	ZeroMemory(&dsDescAlways, sizeof(dsDescAlways));
	dsDescAlways.DepthEnable = true;
	dsDescAlways.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // write 안 함
	dsDescAlways.DepthFunc = D3D11_COMPARISON_ALWAYS;  // 항상 통과

	dsDescAlways.StencilEnable = true;
	dsDescAlways.StencilReadMask = 0xFF;
	dsDescAlways.StencilWriteMask = 0xFF;
	dsDescAlways.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescAlways.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDescAlways.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDescAlways.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDescAlways.BackFace = dsDescAlways.FrontFace;

	hr = pd3dDevice_->CreateDepthStencilState(&dsDescAlways, depthStencilStateAlways_.GetAddressOf());
	assert(SUCCEEDED(hr) && "Depth Always 상태 생성 실패");


	//Rasterizer State 생성 (뒷면 제거 & 와이어 프레임 설정 등)
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));

	rasterDesc.FillMode = D3D11_FILL_SOLID; //wireframe 아님
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FrontCounterClockwise = false; //시계방향이 앞면(DirectX 기본)
	rasterDesc.DepthClipEnable = true;

	hr = pd3dDevice_->CreateRasterizerState(&rasterDesc, solidState_.GetAddressOf());
	assert(SUCCEEDED(hr) && "solid 레스터라이저 생성 실패");
	if (FAILED(hr)) { return false; }

	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.DepthBias = -1000;
	rasterDesc.SlopeScaledDepthBias = -2.0f;
	rasterDesc.DepthBiasClamp = 0.0f;

	hr = pd3dDevice_->CreateRasterizerState(&rasterDesc, wireframeState_.GetAddressOf());
	assert(SUCCEEDED(hr) && "wire 레스터라이저 생성 실패");
	if (FAILED(hr)) { return false; }


	//Blend State 생성
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;

	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = pd3dDevice_->CreateBlendState(&blendDesc, blendStateOpaque_.GetAddressOf());
	assert(SUCCEEDED(hr) && "블렌드 상태 생성 실패");
	if (FAILED(hr)) { return false; }


	//최종 연결
	ID3D11RenderTargetView* rtv = mainRenderTargetView_.Get();
	pd3dDeviceContext_->OMSetRenderTargets(1, &rtv, depthStencilView_.Get());
	pd3dDeviceContext_->OMSetDepthStencilState(depthStencilStateOn_.Get(), 1); //기본 Depth On 설정
	pd3dDeviceContext_->RSSetState(solidState_.Get());
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pd3dDeviceContext_->OMSetBlendState(blendStateOpaque_.Get(), blendFactor, 0xFFFFFFFF);


	//뷰포트 설정
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	pd3dDeviceContext_->RSSetViewports(1, &viewport);

	//기본 셰이더(정적 리소스) 준비
	if (!Render::Material::prepareDefaultShaders(pd3dDevice_.Get())) 
	{
		return false;
	}

	return true;
}

void DX11Renderer::beginFrame(const float* bgColor)
{
	//초기화
	pd3dDeviceContext_->ClearRenderTargetView(mainRenderTargetView_.Get(), bgColor);
	pd3dDeviceContext_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//뷰포트 설정
	RECT clientRect;
	GetClientRect(hwnd_, &clientRect);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(clientRect.right - clientRect.left);
	viewport.Height = static_cast<float>(clientRect.bottom - clientRect.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	pd3dDeviceContext_->RSSetViewports(1, &viewport);

	//3D 렌더링용 설정
	pd3dDeviceContext_->OMSetRenderTargets(1, mainRenderTargetView_.GetAddressOf(), depthStencilView_.Get());
	pd3dDeviceContext_->OMSetDepthStencilState(depthStencilStateOn_.Get(), 1);
	pd3dDeviceContext_->RSSetState(solidState_.Get());
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pd3dDeviceContext_->OMSetBlendState(blendStateOpaque_.Get(), blendFactor, 0xFFFFFFFF);
}

void DX11Renderer::endFrame() 
{
	pSwapChain_->Present(1, 0); //VSync on
}


bool DX11Renderer::createDevice(HWND hwnd) 
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
#if defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	HRESULT hr = E_FAIL;
	for (auto driverType : driverTypes) 
	{
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr, driverType, nullptr, createDeviceFlags, nullptr, 0,
			D3D11_SDK_VERSION, &sd, pSwapChain_.GetAddressOf(), pd3dDevice_.GetAddressOf(), nullptr, pd3dDeviceContext_.GetAddressOf());

		if (SUCCEEDED(hr))
			break;
	}

	return SUCCEEDED(hr);
}

void DX11Renderer::createRenderTarget() 
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	HRESULT hr = pSwapChain_->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()));
	assert(SUCCEEDED(hr) && "렌더타겟 생성에 실패했습니다.");
	pd3dDevice_->CreateRenderTargetView(pBackBuffer.Get(), nullptr, mainRenderTargetView_.GetAddressOf());
}

void DX11Renderer::setDepthEnabled(bool enable) 
{
	if (enable) 
	{
		pd3dDeviceContext_->OMSetDepthStencilState(depthStencilStateOn_.Get(), 1);
	}
	else 
	{
		pd3dDeviceContext_->OMSetDepthStencilState(depthStencilStateOff_.Get(), 1);
	}
}

//스마트 포인터를 사용하지만, 파이프라인 초기화 + 전체화면시 문제 방지를 위해 명시적 선언을 하는 게 좋음
void DX11Renderer::shutdown() 
{
	if (pd3dDeviceContext_) 
	{
		pd3dDeviceContext_->ClearState();
	}

	depthStencilStateOff_.Reset();
	depthStencilStateOn_.Reset();
	depthStencilView_.Reset();
	depthStencilBuffer_.Reset();
	mainRenderTargetView_.Reset();

	//창모드 처리
	if (pSwapChain_) 
	{
		pSwapChain_->SetFullscreenState(FALSE, nullptr);
		pSwapChain_.Reset();
	}

	sceneBuffer_.reset();

	pd3dDeviceContext_.Reset();
	pd3dDevice_.Reset();
}

void DX11Renderer::setWireframeMode(bool enable) 
{
	if (enable) { pd3dDeviceContext_->RSSetState(wireframeState_.Get()); }
	else { pd3dDeviceContext_->RSSetState(solidState_.Get()); }
}

void DX11Renderer::updateSceneConstants(const Render::SceneConstantBufferData& data) 
{
	assert(sceneBuffer_ && "sceneBuffer_가 없습니다. initialize가 먼저 호출되야 합니다.");
	assert(pd3dDeviceContext_ && "디바이스 컨텍스트가 유효하지 않습니다.");
	if (!sceneBuffer_ || !pd3dDeviceContext_) { return; }

	sceneBuffer_->update(pd3dDeviceContext_.Get(), data);

	sceneBuffer_->bindVS(pd3dDeviceContext_.Get(), 0);
	sceneBuffer_->bindPS(pd3dDeviceContext_.Get(), 0);
}

void DX11Renderer::updateLightConstants(const Render::LightBufferData& data)
{
	assert(lightBuffer_ && "lightBuffer_가 없습니다. initialize가 먼저 호출되야 합니다.");
	assert(pd3dDeviceContext_ && "디바이스 컨텍스트가 유효하지 않습니다.");
	if (!lightBuffer_ || !pd3dDeviceContext_) { return; }

	lightBuffer_->update(pd3dDeviceContext_.Get(), data);
	lightBuffer_->bindPS(pd3dDeviceContext_.Get(), 3); //cbuffer b3
}
