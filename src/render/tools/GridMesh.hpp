#pragma once
#include "../Mesh.hpp"
#include "common/Math.hpp"


namespace Render 
{
    namespace Tools 
    {
        class GridMesh : public Mesh 
        {
        public:
            GridMesh();
            virtual ~GridMesh() = default;

            void rebuild(ID3D11Device* device, int gridSize, float gridSpacing);

        private:
            const Math::Vec4 axisColor_ = { 0.8f, 0.8f, 0.8f, 1.0f };
            const Math::Vec4 lineColor_ = { 0.6f, 0.6f, 0.6f, 1.0f };

        };
    }
}
