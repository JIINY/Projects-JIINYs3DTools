#pragma once
#include <d3d11.h>
#include "render/structures/SceneConstantBufferData.hpp"
#include "render/lights/LightType.hpp"


class Renderer {
public:
	virtual ~Renderer() = default;

	virtual bool initialize(void* hwnd) = 0;
	virtual void beginFrame(const float* bgColor) = 0; //화면 지우기+준비
	virtual void endFrame() = 0; //화면 출력
	virtual void shutdown() = 0;

	virtual ID3D11Device* getDevice() = 0;
	virtual ID3D11DeviceContext* getDeviceContext() = 0;

	virtual void onResize(int width, int height) = 0;
	virtual void updateSceneConstants(const Render::SceneConstantBufferData& data) = 0;
	virtual void updateLightConstants(const Render::LightBufferData& data) = 0;
	virtual void setWireframeMode(bool enable) = 0;
};
