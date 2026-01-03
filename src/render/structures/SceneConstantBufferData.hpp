#include "common/Math.hpp"


namespace Render 
{
    struct SceneConstantBufferData
    {
        Math::Mat4 view;
        Math::Mat4 projection;
        Math::Vec3 cameraPos;
        float padding1;

        Math::Vec3 sunDirection;
        float sunIntensity;
        Math::Vec3 sunColor;
        float padding2;

        Math::Vec3 ambientTop;
        float padding3;
        Math::Vec3 ambientMid;
        float padding4;
        Math::Vec3 ambientBot;
        float padding5;
    };
}
