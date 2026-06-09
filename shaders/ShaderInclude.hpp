#pragma once
#include <string>
#include <unordered_map>
#include "render/Material.hpp"

#include "Lit_BlinnPhong/Lit_Preset_BlinnPhong.hpp"
#include "Lit_HalfLambert/Lit_Preset_HalfLambert.hpp"
#include "Lit_PhysicallyBasedRendering/Lit_Preset_PBR.hpp"
#include "Unlit_MaterialColor/Unlit_Preset_MaterialColor.hpp"
#include "Unlit_VertexColor/Unlit_Preset_VertexColor.hpp"


inline std::unordered_map<std::wstring, void(*)(Render::Material&)> presetMap =
{
    {L"Lit_BlinnPhong", Render::Materials::setLitBlinnPhongPreset},
    {L"Lit_HalfLambert", Render::Materials::setLitHalfLambertPreset},
    {L"Lit_PBR", Render::Materials::setLitPBRPreset},
    {L"Unlit_MaterialColor", Render::Materials::setUnlitMaterialColorPreset},
    {L"Unlit_VertexColor", Render::Materials::setUnlitVertexColorPreset},
};
