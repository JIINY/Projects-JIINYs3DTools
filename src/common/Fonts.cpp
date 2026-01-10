#include "Fonts.hpp"
#include "misc/freetype/imgui_freetype.h"
#include <fstream>
#include <vector>
#include <memory>

using namespace std;

namespace Fonts 
{
	static vector<unique_ptr<unsigned char[]>> fontMemoryBlocks;	//절대로 사라지지 않음
	
	ImFont* Regular17 = nullptr;
	ImFont* Regular18 = nullptr;
	ImFont* Light18 = nullptr;
	ImFont* Heavy17 = nullptr;
	ImFont* Heavy20 = nullptr;

	void Init()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->Clear();
		io.Fonts->FontLoader = ImGuiFreeType::GetFontLoader();

		Regular17 = LoadFontFromMemory("resources/fonts/NanumHumanRegular.ttf", 17.0f, "Regular17");
		Regular18 = LoadFontFromMemory("resources/fonts/NanumHumanRegular.ttf", 18.0f, "Regular18");
		Light18 = LoadFontFromMemory("resources/fonts/NanumHumanLight.ttf", 18.0f, "Light18");
		Heavy17 = LoadFontFromMemory("resources/fonts/NunumHumanHeavy.ttf", 17.0f, "Heavy17");
		Heavy20 = LoadFontFromMemory("resources/fonts/NanumHumanHeavy.ttf", 20.0f, "Heavy20");

		io.Fonts->FontLoaderFlags = ImGuiFreeTypeBuilderFlags_LightHinting; //작은 글자를 그릴 때 래스터라이저에 힌팅 가이드라인 제공
		io.FontDefault = Regular18;
	}

	ImFont* LoadFontFromMemory(const char* path, float size_pixels, const char* name)
	{
		ifstream file(path, ios::binary | ios::ate);
		if (!file.is_open()) return nullptr;

		streamsize size = file.tellg();
		file.seekg(0, ios::beg);

		//1. 파일에서 직접 읽은 원본 버퍼
		auto srcBuffer = make_unique<unsigned char[]>(size);
		if (!file.read(reinterpret_cast<char*>(srcBuffer.get()), size)) return nullptr;

		//2. AddFont에 넘길 메모리 메모리 복사본 생성 (같은 폰트 다른 크기면 따로 사용)
		auto bufferCopy = make_unique<unsigned char[]>(size);
		memcpy(bufferCopy.get(), srcBuffer.get(), size);

		//unique_ptr을 vector에 넣어서 수명 보장
		fontMemoryBlocks.push_back(move(bufferCopy));

		ImGuiIO& io = ImGui::GetIO();
		ImFontConfig fConfig;
		fConfig.OversampleH = 1;
		fConfig.OversampleV = 1;
		fConfig.FontDataOwnedByAtlas = false; //빠지면 메모리 이중 해제 발생 가능
		strcpy_s(fConfig.Name, name);

		const ImWchar* glyphRanges = io.Fonts->GetGlyphRangesKorean();

		ImFont* result = io.Fonts->AddFontFromMemoryTTF(fontMemoryBlocks.back().get(),
			static_cast<int>(size), size_pixels, &fConfig, glyphRanges);
		return result;
	}
}
