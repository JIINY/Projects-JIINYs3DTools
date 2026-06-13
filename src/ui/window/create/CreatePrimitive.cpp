#include "CreatePrimitive.hpp"
#include <cassert>
#include "command/CommandStack.hpp"
#include "command/passiveObject/CmdCreatePrimitive.hpp"
#include "core/coordinator/scene/PassiveObjectCoordinator.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;
using namespace Render::Primitives;


namespace Create 
{
    bool CreatePrimitive::initialize(PassiveObjectCoordinator* passiveObjCoord)
    {
        assert(passiveObjCoord && "비었습니다. 초기화 실패");
        if (!passiveObjCoord) { return false; }
        passiveObjCoord_ = passiveObjCoord;

        return true;
    }

    void CreatePrimitive::draw()
    {
        if (ImGui::CollapsingHeader("Shapes", ImGuiTreeNodeFlags_DefaultOpen))
        {
            float currentWidth = ImGui::GetContentRegionAvail().x;
            float spacing = 2.0f;
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, 0));
            float btnWidth3 = (currentWidth - (spacing * 2)) / 3.0f;
            float btnHeight = ImGui::GetFrameHeight();

            auto selectType = [&](const char* label, PrimitiveType type) 
            {
                if (ImGui::Button(label, ImVec2(btnWidth3, btnHeight + 1))) 
                {
                    PrimitiveData data = setDefaultData(type);
                    auto cmd = make_shared<CmdCreatePrimitive>(passiveObjCoord_, data);
                    CommandStack::get().execute(cmd);
                }
            };

            selectType("Box", PrimitiveType::Cube); ImGui::SameLine();
            selectType("Cylinder", PrimitiveType::Cylinder); ImGui::SameLine();
            selectType("Plane", PrimitiveType::Plane);

            ImGui::Dummy(ImVec2(0.0f, 3.0f));

            selectType("Sphere", PrimitiveType::Sphere); ImGui::SameLine();
            selectType("Capsule", PrimitiveType::Capsule); ImGui::SameLine();
            if (ImGui::Button("Ring", ImVec2(btnWidth3, btnHeight + 1))) 
            {
                //TO DO: OBJ Loader 연결
                //테스트용 Torus 연결
                PrimitiveData data = setDefaultData(PrimitiveType::Torus);
                auto cmd = make_shared<CmdCreatePrimitive>(passiveObjCoord_, data);
                CommandStack::get().execute(cmd);
            }
            ImGui::PopStyleVar();
        }
    }









    /*

    void CreatePrimitive::drawSettings() 
    {
        switch (selectedType_) 
        {
        case PrimitiveType::Cube: drawCubeSettings(); break;
        case PrimitiveType::Cylinder: drawCylinderSettings(); break;
        case PrimitiveType::Plane: drawPlaneSettings(); break;
        case PrimitiveType::Sphere: drawSphereSettings(); break;
        case PrimitiveType::Capsule: drawCapsuleSettings(); break;
        default: break;
        }
    }


    void CreatePrimitive::drawCubeSettings() 
    {
        static ImGuiTableFlags flags = ImGuiTableFlags_None;
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(1.0f, 2.0f));
        if (ImGui::BeginTable("SizeTable", 4, flags))
        {
            ImGui::TableSetupColumn("SizeLabel", ImGuiTableColumnFlags_WidthFixed, 93.0f);
            ImGui::TableSetupColumn("SizeValue", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("SizeMinus", ImGuiTableColumnFlags_WidthFixed, 28.0f);
            ImGui::TableSetupColumn("SizePlus", ImGuiTableColumnFlags_WidthFixed, 28.0f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Width");
            ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::DragFloat("##Width", &pendingData_.width, 0.1f, 1.0f);

            ImGui::TableNextColumn();
            if (ImGui::Button("-##WidthM", ImVec2(-FLT_MIN, 0)))
            {
                if (pendingData_.width > 1) { pendingData_.width--; }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("+##WidthP", ImVec2(-FLT_MIN, 0))) { pendingData_.width++; }


            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Depth");
            ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::DragFloat("##Depth", &pendingData_.depth, 0.1f, 1.0f);

            ImGui::TableNextColumn();
            if (ImGui::Button("-##DepthM", ImVec2(-FLT_MIN, 0)))
            {
                if (pendingData_.depth > 1) { pendingData_.depth--; }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("+##DepthP", ImVec2(-FLT_MIN, 0))) { pendingData_.depth++; }


            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Height");
            ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::DragFloat("##Height", &pendingData_.height, 0.1f, 1.0f);

            ImGui::TableNextColumn();
            if (ImGui::Button("-##HeightM", ImVec2(-FLT_MIN, 0)))
            {
                if (pendingData_.height > 1) { pendingData_.height--; }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("+##HeightP", ImVec2(-FLT_MIN, 0))) { pendingData_.height++; }
            ImGui::EndTable();
        }

        ImGui::Dummy(ImVec2(0.1f, 1.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.1f, 1.0f));

        if (ImGui::BeginTable("SegmentTable", 4, flags))
        {
            ImGui::TableSetupColumn("SegmentsLabel", ImGuiTableColumnFlags_WidthFixed, 93.0f);
            ImGui::TableSetupColumn("SegmentsValue", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("SegmentsMinus", ImGuiTableColumnFlags_WidthFixed, 28.0f);
            ImGui::TableSetupColumn("SegmentsPlus", ImGuiTableColumnFlags_WidthFixed, 28.0f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Segments X");
            ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::DragInt("##SegementX", &pendingData_.segmentsX, 1, 1);

            ImGui::TableNextColumn();
            if (ImGui::Button("-##SegmentXM", ImVec2(-FLT_MIN, 0)))
            {
                if (pendingData_.segmentsX > 1) { pendingData_.segmentsX--; }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("+##SegmentXP", ImVec2(-FLT_MIN, 0))) { pendingData_.segmentsX++; }


            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Segments Y");
            ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::DragInt("##SegementY", &pendingData_.segmentsY, 1, 1);

            ImGui::TableNextColumn();
            if (ImGui::Button("-##SegmentYM", ImVec2(-FLT_MIN, 0)))
            {
                if (pendingData_.segmentsY > 1) { pendingData_.segmentsY--; }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("+##SegmentYP", ImVec2(-FLT_MIN, 0))) { pendingData_.segmentsY++; }


            ImGui::TableNextRow(); 
            ImGui::TableNextColumn(); ImGui::Text("Segments Z");
            ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::DragInt("##SegementZ", &pendingData_.segmentsZ, 1, 1);

            ImGui::TableNextColumn();
            if (ImGui::Button("-##SegmentZM", ImVec2(-FLT_MIN, 0)))
            {
                if (pendingData_.segmentsZ > 1) { pendingData_.segmentsZ--; }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("+##SegmentZP", ImVec2(-FLT_MIN, 0))) { pendingData_.segmentsZ++; }

            ImGui::EndTable();
        }
        ImGui::PopStyleVar();
    }

    void CreatePrimitive::drawCylinderSettings()
    {
        static ImGuiTableFlags flags = ImGuiTableFlags_None;
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(1.0f, 2.0f));
        if (ImGui::BeginTable("SizeTable", 4, flags))
        {
            ImGui::TableSetupColumn("SizeLabel", ImGuiTableColumnFlags_WidthFixed, 93.0f);
            ImGui::TableSetupColumn("SizeValue", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("SizeMinus", ImGuiTableColumnFlags_WidthFixed, 28.0f);
            ImGui::TableSetupColumn("SizePlus", ImGuiTableColumnFlags_WidthFixed, 28.0f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Radius Top");
            ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::DragFloat("##RadiusTop", &pendingData_.radiusTop, 0.05f, 0.0f);

            ImGui::TableNextColumn();
            if (ImGui::Button("-##RadiusTopM", ImVec2(-FLT_MIN, 0)))
            {
                if (pendingData_.radiusTop > 1) { pendingData_.radiusTop--; }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("+##RadiusTopP", ImVec2(-FLT_MIN, 0))) { pendingData_.radiusTop++; }


            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Radius Bot");
            ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::DragFloat("##RadiusBot", &pendingData_.radiusBot, 0.05f, 0.0f);

            ImGui::TableNextColumn();
            if (ImGui::Button("-##RadiusBotM", ImVec2(-FLT_MIN, 0)))
            {
                if (pendingData_.radiusBot > 1) { pendingData_.radiusBot--; }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("+##RadiusBotP", ImVec2(-FLT_MIN, 0))) { pendingData_.radiusBot++; }


            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Height");
            ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::DragFloat("##Height", &pendingData_.height, 0.1f, 0.1f);

            ImGui::TableNextColumn();
            if (ImGui::Button("-##HeightM", ImVec2(-FLT_MIN, 0)))
            {
                if (pendingData_.height > 1) { pendingData_.height--; }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("+##HeightP", ImVec2(-FLT_MIN, 0))) { pendingData_.height++; }

            ImGui::EndTable();
        }

        ImGui::Dummy(ImVec2(0.1f, 1.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.1f, 1.0f));

        if (ImGui::BeginTable("SegmentTable", 4, flags))
        {
            ImGui::TableSetupColumn("SegmentsLabel", ImGuiTableColumnFlags_WidthFixed, 93.0f);
            ImGui::TableSetupColumn("SegmentsValue", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("SegmentsMinus", ImGuiTableColumnFlags_WidthFixed, 28.0f);
            ImGui::TableSetupColumn("SegmentsPlus", ImGuiTableColumnFlags_WidthFixed, 28.0f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Segments X");
            ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::DragInt("##SegementX", &pendingData_.segmentsX, 1, 3, 64);

            ImGui::TableNextColumn();
            if (ImGui::Button("-##SegmentXM", ImVec2(-FLT_MIN, 0)))
            {
                if (pendingData_.segmentsX > 3) { pendingData_.segmentsX--; }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("+##SegmentXP", ImVec2(-FLT_MIN, 0))) { pendingData_.segmentsX++; }


            ImGui::TableNextRow();
            ImGui::TableNextColumn(); ImGui::Text("Segments Y");
            ImGui::TableNextColumn(); ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::DragInt("##SegementY", &pendingData_.segmentsY, 1, 1, 64);

            ImGui::TableNextColumn();
            if (ImGui::Button("-##SegmentYM", ImVec2(-FLT_MIN, 0)))
            {
                if (pendingData_.segmentsY > 1) { pendingData_.segmentsY--; }
            }
            ImGui::TableNextColumn();
            if (ImGui::Button("+##SegmentYP", ImVec2(-FLT_MIN, 0))) { pendingData_.segmentsY++; }

            ImGui::EndTable();
        }
        ImGui::PopStyleVar();
    }

    void CreatePrimitive::drawPlaneSettings()
    {
        ImGui::DragFloat("Width", &pendingData_.width, 0.5f, 1.0f, 500.0f);
        ImGui::DragFloat("Depth", &pendingData_.depth, 0.5f, 1.0f, 500.0f);
        ImGui::DragInt("Segement X", &pendingData_.segmentsX, 1, 1, 100);
        ImGui::DragInt("Segement Y", &pendingData_.segmentsY, 1, 1, 100);
    }

    void CreatePrimitive::drawSphereSettings()
    {
        ImGui::DragFloat("Radius", &pendingData_.radius, 0.05f, 0.1f, 50.0f);
        ImGui::DragInt("Segement X", &pendingData_.segmentsX, 1, 3, 64);
        ImGui::DragInt("Segement Y", &pendingData_.segmentsY, 1, 3, 64);
    }

    void CreatePrimitive::drawCapsuleSettings()
    {
        ImGui::DragFloat("Radius", &pendingData_.radius, 0.05f, 0.1f, 50.0f);
        ImGui::DragFloat("Height Body", &pendingData_.height, 0.1f, 0.1f, 100.0f);
        ImGui::DragFloat("Height Hemi", &pendingData_.heightHemi, 0.05f, 0.1f, 50.0f);
        ImGui::DragInt("Segments X", &pendingData_.segmentsX, 1, 3, 64);
        ImGui::DragInt("Segments Y", &pendingData_.segmentsY, 1, 3, 64);
        ImGui::DragInt("Segments Hemi", &pendingData_.segmentsHemi, 1, 1, 32);
    }

    void CreatePrimitive::drawTeapotSettings() {}
    */
}
