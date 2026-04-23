#pragma once
#include <DirectXMath.h>
#include "LightType.hpp"


namespace Render 
{
    class Light 
    {
    public:
        Light(LightType type) { data_.type = static_cast<int>(type); }
        virtual ~Light() = default;

        void setPosition(const DirectX::XMFLOAT3& pos) { data_.pos = pos; }
        void setDirection(const DirectX::XMFLOAT3& dir) { data_.direction = dir; }
        void setColor(const DirectX::XMFLOAT3& color) { data_.color = color; }
        void setIntensity(float intensity) { data_.intensity = intensity; }
        void setRange(float range) { data_.range = range; }
        void setSpotAngle(float angle) { data_.spotAngle = angle; }

        DirectX::XMFLOAT3 getPosition() const { return data_.pos; }
        DirectX::XMFLOAT3 getDirection() const { return data_.direction; }
        DirectX::XMFLOAT3 getColor() const { return data_.color; }
        float getIntensity() const { return data_.intensity; }
        float getRange() const { return data_.range; }
        float getSpotAngle() const { return data_.spotAngle; }
        LightType getType() const { return static_cast<LightType>(data_.type); }

        const LightData& getData() const { return data_; }

    protected:
        LightData data_;
    };
}
