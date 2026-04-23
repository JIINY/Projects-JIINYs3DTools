#include "ShaderManager.hpp"
#include <cassert>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include "render/VertexShader.hpp"
#include "render/PixelShader.hpp"
using namespace std;
using namespace Render;


bool ShaderManager::initialize(ID3D11Device* device)
{
	assert(device && "ShaderManager 초기화 실패: 디바이스가 null입니다.");
	if (!device) { return false; }

	device_ = device;
	return true;
}

void ShaderManager::shutdown() 
{
	vsCache_.clear();
	psCache_.clear();
	device_ = nullptr;
}

shared_ptr<Render::VertexShader> ShaderManager::getVertexShader(const wstring& path, const string& entryPoint)
{
	//1. 캐시 키 생성
	wstring wEntry(entryPoint.begin(), entryPoint.end());
	wstring cacheKey = path + L"|" + wEntry;

	//2. 캐시 검색
	auto it = vsCache_.find(cacheKey); //find가 map[key]보다 빠름	
	if (it != vsCache_.end()) { return it->second; } //찾았으면 캐싱된 셰이더 반환
	
	//3. 없으면 새로 생성
	auto newShader = make_shared<VertexShader>();
	if (!newShader->initialize(device_, path, entryPoint))
	{
		assert(0 && "VertexShader 로드 실패");
		vsCache_[cacheKey] = nullptr; //실패시 nullptr를 캐시에 저장해서 중복로드X
		return nullptr;
	}

	//4. 캐시 등록 및 반환
	vsCache_[cacheKey] = newShader;
	return newShader;
}

shared_ptr<Render::PixelShader> ShaderManager::getPixelShader(const wstring& path, const string& entryPoint) 
{
	wstring wEntry(entryPoint.begin(), entryPoint.end());
	wstring cacheKey = path + L"|" + wEntry;

	auto it = psCache_.find(cacheKey); //find가 map[key]보다 빠름	
	if (it != psCache_.end()) { return it->second; }

	auto newShader = make_shared<PixelShader>();
	if (!newShader->initialize(device_, path, entryPoint))
	{
		assert(0 && "PixelShader 로드 실패");
		psCache_[cacheKey] = nullptr;
		return nullptr;
	}

	psCache_[cacheKey] = newShader;
	return newShader;
}
