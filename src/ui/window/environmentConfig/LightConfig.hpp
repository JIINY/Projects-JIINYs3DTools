#pragma once

class LightManager;


namespace EnvConfig 
{
    class LightConfig
    {
    public:
        void initialize(LightManager* manager);
        void draw();

    private:
        LightManager* manager_ = nullptr;

        float sunTheta_ = 0.0;
        float sunPhi_ = 45.0f;
        float sunColor_[3] = { 1.0f, 1.0f, 1.0f };
        float sunIntensity_ = 1.0f;

        float ambientTop_[3] = { 0.75f, 0.75f, 0.75f };
        float ambientMid_[3] = { 0.5f, 0.5f, 0.5f };
        float ambientBot_[3] = { 0.25f, 0.25f, 0.25f };
    };
}
