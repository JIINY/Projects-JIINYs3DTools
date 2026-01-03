#include "LightManager.hpp"


bool LightManager::initialize(const GlobalLightConfig& config)
{
    dirLight_ = std::make_shared<Render::DirectionLight>();
    dirLight_->setDirection(config.sunDirection);
    dirLight_->setColor(config.sunColor);
    dirLight_->setIntensity(config.sunIntensity);

    ambientTop_ = config.ambientTop;
    ambientMid_ = config.ambientMid;
    ambientBot_ = config.ambientBot;

    return true;
}

Render::LightData LightManager::getDirectionalLightData() const 
{
    if (!dirLight_) 
    { 
        return Render::LightData(); 
    }
    return dirLight_->getData();
}

void LightManager::setDirectionalLightDir(const Math::Vec3& dir) 
{
    if (dirLight_) { dirLight_->setDirection(dir); }
}

void LightManager::setDirectionalLightColor(const Math::Vec3& color) 
{
    if (dirLight_) { dirLight_->setColor(color); }
}

void LightManager::setDirectionalLightIntensity(const float intensity) 
{
    if (dirLight_) { dirLight_->setIntensity(intensity); }
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
    pointLights_.clear();
    spotLights_.clear();
}
