#pragma once
#include "imgui.h"


namespace Fonts 
{
	extern ImFont* Regular17;
	extern ImFont* Regular18;
	extern ImFont* Light18;
	extern ImFont* Heavy17;
	extern ImFont* Heavy20;


	void Init();

	ImFont* LoadFontFromMemory(const char* path, float size_pixels, const char* name);
}
