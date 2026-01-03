#include "LightConfig.hpp"
#include <cmath>
#include <algorithm>
#include <DirectXMath.h>
#include "common/Math.hpp"
#include "core/manager/scene/LightManager.hpp"
#include "imgui.h"
using namespace std;


namespace EnvConfig 
{
    void LightConfig::initialize(LightManager* manager)
    {
        manager_ = manager;
        if (!manager_) { return; }

        auto sun = manager_->getDirectionalLight();
        Math::Vec3 dir = sun->getDirection();
        float clampY = clamp(dir.y, -1.0f, 1.0f);

        float radPhi = asinf(clampY);
        sunPhi_ = DirectX::XMConvertToDegrees(radPhi);

        float radTheta = atan2f(dir.x, dir.z);
        sunTheta_ = DirectX::XMConvertToDegrees(radTheta);

        if (sunTheta_ < 0.0f) { sunTheta_ += 360.0f; }

        auto sunColor = sun->getColor();
        sunColor_[0] = sunColor.x;
        sunColor_[1] = sunColor.y;
        sunColor_[2] = sunColor.z;

        sunIntensity_ = sun->getIntensity();

        auto [top, mid, bot] = manager_->getAmbientColors();
        ambientTop_[0] = top.x; ambientTop_[1] = top.y; ambientTop_[2] = top.z;
        ambientMid_[0] = mid.x; ambientMid_[1] = mid.y; ambientMid_[2] = mid.z;
        ambientBot_[0] = bot.x; ambientBot_[1] = bot.y; ambientBot_[2] = bot.z;
    }

    void LightConfig::draw() 
    {
        if (!manager_) { return; }

        static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchProp;
        if (ImGui::CollapsingHeader("SunLight", ImGuiTreeNodeFlags_DefaultOpen)) 
        {
            if (ImGui::BeginTable("SunTable", 2, flags)) 
            {
                ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch, 0.3f);
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 0.7f);

                bool sunChanged = false;

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Rotation");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(-FLT_MIN);
                sunChanged |= ImGui::SliderFloat("##Rotation", &sunTheta_, 0.0f, 360.0f);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Height");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(-FLT_MIN);
                sunChanged |= ImGui::SliderFloat("##Height", &sunPhi_, 0.0f, 90.0f);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Intensity");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(-FLT_MIN);
                sunChanged |= ImGui::SliderFloat("##Intensity", &sunIntensity_, 0.01f, 5.0f);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Color");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(-FLT_MIN);
                sunChanged |= ImGui::ColorEdit3("##Color", sunColor_);

                ImGui::EndTable();
                if (sunChanged)
                {
                    float radTheta = DirectX::XMConvertToRadians(sunTheta_);
                    float radPhi = DirectX::XMConvertToRadians(sunPhi_);

                    Math::Vec3 dir = { cos(radPhi) * sin(radTheta), sin(radPhi), cos(radPhi) * cos(radTheta) };
                    dir = Math::Normalize(dir);

                    manager_->setDirectionalLightDir(dir);
                    manager_->setDirectionalLightColor({ sunColor_[0], sunColor_[1], sunColor_[2] });
                    manager_->setDirectionalLightIntensity(sunIntensity_);
                }
            }
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("AmbientLight", ImGuiTreeNodeFlags_DefaultOpen)) 
        {
            bool ambChanged = false;

            ambChanged |= ImGui::ColorEdit3("Sky", ambientTop_);
            ambChanged |= ImGui::ColorEdit3("Horizon", ambientMid_);
            ambChanged |= ImGui::ColorEdit3("Ground", ambientBot_);

            if (ambChanged) 
            {
                manager_->setAmbientTop({ ambientTop_[0], ambientTop_[1], ambientTop_[2] });
                manager_->setAmbientMid({ ambientMid_[0], ambientMid_[1], ambientMid_[2] });
                manager_->setAmbientBot({ ambientBot_[0], ambientBot_[1], ambientBot_[2] });
            }
        }
    }

}
