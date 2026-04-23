#include "LightConfig.hpp"
#include <string>
#include <cmath>
#include <algorithm>
#include <DirectXMath.h>
#include "common/Math.hpp"
#include "core/manager/scene/LightManager.hpp"
#include "render/lights/DirectionalLight.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;


namespace EnvConfig 
{
    void LightConfig::initialize(LightManager* manager)
    {
        manager_ = manager;
        setFromManager();
    }

    bool LightConfig::draw() 
    {
        bool isChanged = false;
        if (!manager_) { return false; }

        static ImGuiTableFlags flags = ImGuiTableFlags_None;
        if (ImGui::CollapsingHeader("SunLight", ImGuiTreeNodeFlags_DefaultOpen)) 
        {
            if (ImGui::BeginTable("SunTable", 2, flags)) 
            {
                ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch, 0.28f);
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 0.72f);

                bool sunChanged = false;

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Rotation");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(-FLT_MIN);

                int rotationStep = (int)(sunTheta_ / 5.0f);
                char rotLabel[32];
                sprintf_s(rotLabel, sizeof(rotLabel), "%d", rotationStep * 5);
                if (ImGui::SliderInt("##Rotation", &rotationStep, 0, 72, rotLabel))
                {
                    sunTheta_ = (float)rotationStep * 5.0f;
                    sunChanged = true;
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Height");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(-FLT_MIN);

                int heightStep = (int)(sunPhi_ / 2.5f);
                char heightLabel[32];
                sprintf_s(heightLabel, sizeof(heightLabel), "%.1f", heightStep * 2.5f);
                if (ImGui::SliderInt("##Height", &heightStep, 0, 36, heightLabel))
                {
                    sunPhi_ = (float)heightStep * 2.5f;
                    sunChanged = true;
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Intensity");
                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(-FLT_MIN);

                int intensityStep = (int)(sunIntensity_ / 0.2f);
                char intensityLabel[32];
                sprintf_s(intensityLabel, sizeof(intensityLabel), "%.1f", intensityStep * 0.2f);
                if (ImGui::SliderInt("##Intensity", &intensityStep, 0, 25, intensityLabel))
                {
                    sunIntensity_ = (float)intensityStep * 0.2f;
                    sunChanged = true;
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Color");
                ImGui::TableNextColumn();

                ImGui::SetNextItemWidth(171);
                sunChanged |= ImGui::ColorEdit3("##Color", (float*)&sunColor_, ImGuiColorEditFlags_NoSmallPreview);
                ImGui::SameLine(0, 3.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                sunChanged |= ImGui::ColorEdit3("##ColorPreview", (float*)&sunColor_, ImGuiColorEditFlags_NoInputs);
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();

                ImGui::EndTable();
                if (sunChanged)
                {
                    float radTheta = DirectX::XMConvertToRadians(sunTheta_);
                    float radPhi = DirectX::XMConvertToRadians(sunPhi_);

                    Math::Vec3 dir = { cos(radPhi) * sin(radTheta), sin(radPhi), cos(radPhi) * cos(radTheta) };
                    dir = Math::normalize(dir);

                    manager_->setDirectionalLightDir(dir);
                    manager_->setDirectionalLightColor({ sunColor_[0], sunColor_[1], sunColor_[2] });
                    manager_->setDirectionalLightIntensity(sunIntensity_);

                    isChanged = true;
                }
            }
        }


        if (ImGui::CollapsingHeader("AmbientLight", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::BeginTable("SunTable", 2, flags))
            {
                ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch, 0.28f);
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 0.72f);

                bool ambChanged = false;

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Sky");
                ImGui::TableNextColumn();

                ImGui::SetNextItemWidth(171);
                ambChanged |= ImGui::ColorEdit3("##Sky", (float*)&ambientTop_, ImGuiColorEditFlags_NoSmallPreview);
                ImGui::SameLine(0, 3.0f);
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                ambChanged |= ImGui::ColorEdit3("##SkyPreview", (float*)&ambientTop_, ImGuiColorEditFlags_NoInputs);
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();


                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Horizon");
                ImGui::TableNextColumn();

                ImGui::SetNextItemWidth(171);
                ambChanged |= ImGui::ColorEdit3("##Horizon", (float*)&ambientMid_, ImGuiColorEditFlags_NoSmallPreview);
                ImGui::SameLine(0, 3.0f);
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                ambChanged |= ImGui::ColorEdit3("##HorizonPreview", (float*)&ambientMid_, ImGuiColorEditFlags_NoInputs);
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();


                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Ground");
                ImGui::TableNextColumn();

                ImGui::SetNextItemWidth(171);
                ambChanged |= ImGui::ColorEdit3("##Ground", (float*)&ambientBot_, ImGuiColorEditFlags_NoSmallPreview);
                ImGui::SameLine(0, 3.0f);
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                ambChanged |= ImGui::ColorEdit3("##GroundPreview", (float*)&ambientBot_, ImGuiColorEditFlags_NoInputs);
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();


                ImGui::EndTable();
                if (ambChanged)
                {
                    manager_->setAmbientTop({ ambientTop_[0], ambientTop_[1], ambientTop_[2] });
                    manager_->setAmbientMid({ ambientMid_[0], ambientMid_[1], ambientMid_[2] });
                    manager_->setAmbientBot({ ambientBot_[0], ambientBot_[1], ambientBot_[2] });

                    isChanged = true;
                }
            }
        }

        if (isChanged) { return true; }
        return false;
    }

    void LightConfig::setFromManager() 
    {
        if (!manager_) return;
        auto sun = manager_->getDirectionalLight();

        Math::Vec3 dir = sun->getDirection();
        sunTheta_ = getDegreeThetaFromRad(dir.x, dir.z);
        sunPhi_ = getDegreePhiFromRad(dir.y);

        Math::Vec3 sunColor = sun->getColor();
        sunColor_[0] = sunColor.x;
        sunColor_[1] = sunColor.y;
        sunColor_[2] = sunColor.z;

        sunIntensity_ = sun->getIntensity();

        auto [top, mid, bot] = manager_->getAmbientColors();
        ambientTop_[0] = top.x; ambientTop_[1] = top.y; ambientTop_[2] = top.z;
        ambientMid_[0] = mid.x; ambientMid_[1] = mid.y; ambientMid_[2] = mid.z;
        ambientBot_[0] = bot.x; ambientBot_[1] = bot.y; ambientBot_[2] = bot.z;
    }

    float LightConfig::getDegreePhiFromRad(float dirY)
    {
        float clampY = clamp(dirY, -1.0f, 1.0f);
        float radPhi = asinf(clampY);
        return DirectX::XMConvertToDegrees(radPhi);
    }

    float LightConfig::getDegreeThetaFromRad(float dirX, float dirZ)
    {
        float radTheta = atan2f(dirX, dirZ);
        float degTheta = DirectX::XMConvertToDegrees(radTheta);

        if (degTheta < 0.0f) { degTheta += 360.0f; }
        return degTheta;
    }
}
