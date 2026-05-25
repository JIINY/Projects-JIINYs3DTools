#include "LightManager.hpp"
#include <memory>
#include "render/lights/LightType.hpp"
#include "render/lights/DirectionalLight.hpp"
#include "render/lights/PointLight.hpp"
#include "render/lights/SpotLight.hpp"
using namespace std;


LightManager::LightManager() : sun_(make_unique<Render::DirectionalLight>()) {}
LightManager::~LightManager() = default;

bool LightManager::initialize(const GlobalLightConfig& config)
{
    sun_->setDirection(config.sunDirection);
    sun_->setColor(config.sunColor);
    sun_->setIntensity(config.sunIntensity);

    ambientTop_ = config.ambientTop;
    ambientMid_ = config.ambientMid;
    ambientBot_ = config.ambientBot;

    return true;
}

GlobalLightConfig LightManager::backupCurrentGlobalLightData() const 
{
    GlobalLightConfig config;
    if (sun_)
    {
        auto sun = getSun();
        config.sunDirection = sun->getDirection();
        config.sunColor = sun->getColor();
        config.sunIntensity = sun->getIntensity();
    }

    auto [top, mid, bot] = this->getAmbientColors();
    config.ambientTop = top;
    config.ambientMid = mid;
    config.ambientBot = bot;

    return config;
}

void LightManager::restoreCurrentGlobalLightData(const GlobalLightConfig& config) 
{
    if (sun_) 
    {
        setSunDir(config.sunDirection);
        setSunColor(config.sunColor);
        setSunIntensity(config.sunIntensity);

        setAmbientTop(config.ambientTop);
        setAmbientMid(config.ambientMid);
        setAmbientBot(config.ambientBot);
    }
    else 
    {
        assert(0);
    }
}

Render::LightData LightManager::getSunData() const 
{
    if (!sun_) 
    { 
        return Render::LightData(); 
    }
    return sun_->getData();
}

std::shared_ptr<Render::DirectionalLight> LightManager::addDirectionalLight(const LocalLightConfig& config)
{
    auto light = make_shared<Render::DirectionalLight>();

    light->setDirection(config.direction);
    light->setColor(config.color);
    light->setIntensity(config.intensity);

    dirLights_.push_back(light);
    return light;
}

std::shared_ptr<Render::PointLight> LightManager::addPointLight(const LocalLightConfig& config)
{
    auto light = std::make_shared<Render::PointLight>();

    light->setPosition(config.position);
    light->setColor(config.color);
    light->setIntensity(config.intensity);
    light->setRange(config.range);

    pointLights_.push_back(light);
    return light;
}

std::shared_ptr<Render::SpotLight> LightManager::addSpotLight(const LocalLightConfig& config) 
{
    auto light = std::make_shared<Render::SpotLight>();

    light->setPosition(config.position);
    light->setDirection(config.direction);
    light->setColor(config.color);
    light->setIntensity(config.intensity);
    light->setRange(config.range);
    light->setSpotAngle(config.spotAngle);

    spotLights_.push_back(light);
    return light;
}

void LightManager::removeAllLights() 
{
    dirLights_.clear();
    pointLights_.clear();
    spotLights_.clear();
}

Render::LightBufferData LightManager::getLightBufferData() const
{
    Render::LightBufferData data;
    int& count = data.lightCount;

    for (const auto& light : dirLights_)
    {
        if (count >= Render::MAX_LIGHTS) { break; } //forward 한시 구현. deffered구현시 설계 자체가 변경될 것
        data.lights[count++] = light->getData();
    }

    for(const auto& light : pointLights_)
    {
        if (count >= Render::MAX_LIGHTS) { break; }
        data.lights[count++] = light->getData();
    }

    for (const auto& light : spotLights_)
    {
        if (count >= Render::MAX_LIGHTS) { break; }
        data.lights[count++] = light->getData();
    }

    return data;
}
