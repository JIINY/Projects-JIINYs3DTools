#pragma once
#include <memory>
#include <vector>
#include <tuple>
#include "common/Math.hpp"
#include "render/lights/LightType.hpp"
#include "render/lights/DirectionalLight.hpp"

namespace Render 
{ 
    class DirectionalLight; 
    class PointLight;
    class SpotLight;
}


struct GlobalLightConfig 
{
    //태양
    Math::Vec3 sunDirection = { -0.5f, 1.0f, -0.5f };
    Math::Vec3 sunColor = { 1.0f, 0.95f, 0.8f };
    float sunIntensity = 1.0f;

    //환경광(Hemispheric Ambient): Top/Middle/Bottom 3점 조명
    Math::Vec3 ambientTop = { 0.1f, 0.15f, 0.25f };
    Math::Vec3 ambientMid = { 0.1f, 0.1f, 0.1f };
    Math::Vec3 ambientBot = { 0.05f, 0.05f, 0.05f };
};

struct LocalLightConfig 
{
    Math::Vec3 position = { 0.0f, 0.0f, 0.0f };
    Math::Vec3 direction = { 0.0f, 0.0f, -1.0f };
    Math::Vec3 color = { 1.0f, 1.0f, 1.0f };
    float intensity = 1.0f;
    float range = 10.0f;
    float spotAngle = 30.0f;
};

class LightManager 
{
public:
    LightManager();
    ~LightManager();

    bool initialize(const GlobalLightConfig& config = GlobalLightConfig());
    Render::LightData getSunData() const;
    std::tuple<Math::Vec3, Math::Vec3, Math::Vec3> getAmbientColors() const { return { ambientTop_, ambientMid_, ambientBot_ }; }
    
    GlobalLightConfig backupCurrentGlobalLightData() const;
    void restoreCurrentGlobalLightData(const GlobalLightConfig& config);

    void setSunDir(const Math::Vec3& dir) { sun_->setDirection(dir); }
    void setSunColor(const Math::Vec3& color) { sun_->setColor(color); }
    void setSunIntensity(const float intensity) { sun_->setIntensity(intensity); }

    void setAmbientTop(const Math::Vec3& color) { ambientTop_ = color; }
    void setAmbientMid(const Math::Vec3& color) { ambientMid_ = color; }
    void setAmbientBot(const Math::Vec3& color) { ambientBot_ = color; }

    Render::DirectionalLight* getSun() const { return sun_.get(); }
    const std::vector<std::shared_ptr<Render::DirectionalLight>>& getDirectionalLights() const { return dirLights_; }
    const std::vector<std::shared_ptr<Render::PointLight>>& getPointLights() const { return pointLights_; }
    const std::vector<std::shared_ptr<Render::SpotLight>>& getSpotLights() const { return spotLights_; }
    Render::LightBufferData getLightBufferData() const;

    std::shared_ptr<Render::DirectionalLight> addDirectionalLight(const LocalLightConfig& config = LocalLightConfig());
    std::shared_ptr<Render::PointLight> addPointLight(const LocalLightConfig& config = LocalLightConfig());
    std::shared_ptr<Render::SpotLight> addSpotLight(const LocalLightConfig& config = LocalLightConfig());
    void removeAllLights();

private:
    std::unique_ptr<Render::DirectionalLight> sun_ = nullptr;
    Math::Vec3 ambientTop_ = { 0.0f, 0.0f, 0.0f };
    Math::Vec3 ambientMid_ = { 0.0f, 0.0f, 0.0f };
    Math::Vec3 ambientBot_ = { 0.0f, 0.0f, 0.0f };

    //forward한정. defferred 변경시 재설계 필요
    std::vector<std::shared_ptr<Render::DirectionalLight>> dirLights_ = {};
    std::vector<std::shared_ptr<Render::PointLight>> pointLights_ = {};
    std::vector<std::shared_ptr<Render::SpotLight>> spotLights_ = {};
};
