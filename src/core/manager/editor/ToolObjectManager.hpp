#pragma once
#include <vector>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>


namespace Render::Tools { class Grid; }
namespace Render { class RenderObject; }
class Renderer;


class ToolObjectManager 
{
public:
    static ToolObjectManager* get();
    void initialize(Renderer* renderer);
    void draw(Renderer* renderer, const DirectX::XMMATRIX& viewMat, const DirectX::XMMATRIX& projMat);

    std::shared_ptr<Render::Tools::Grid> createGrid(int size, float spacing);

private:
    ToolObjectManager() = default;
    
    bool isInitialized_ = false;
    ID3D11Device* device_ = nullptr;
    ID3D11DeviceContext* context_ = nullptr;

    std::vector<std::shared_ptr<Render::RenderObject>> tools_;
};