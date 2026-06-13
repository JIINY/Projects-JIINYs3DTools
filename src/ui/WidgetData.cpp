#include "WidgetData.hpp"
#include <cmath>
#include <variant>
#include "imgui.h"
using namespace std;


namespace UI
{
	bool isValidWidgetSettings(WidgetType widget, const WidgetSettings& settings)
	{
		switch (widget)
		{
		case WidgetType::FloatDrag:
			return holds_alternative<FloatDragData>(settings);
		case WidgetType::FloatSlider:
			return holds_alternative<FloatSliderData>(settings);
		case WidgetType::FloatDragnSlider:
			return holds_alternative<FloatDragnSliderData>(settings);
		case WidgetType::Float3Color:
			return holds_alternative<Float3ColorData>(settings);
		case WidgetType::Float4Color:
			return holds_alternative<Float4ColorData>(settings);
		default:
			return false;
		}
	}

	bool drawFloatDrag(const string& label, float& value, const FloatDragData& settings, bool hideLabel, std::string format, bool* outActivated, bool* outDeactivated)
	{
		std::string l = hideLabel ? ("##" + label) : label;

		bool changed = ImGui::DragFloat(l.c_str(), &value, settings.speed, settings.min, settings.max, settings.format.c_str());
		if (outActivated) { *outActivated = ImGui::IsItemActivated(); }
		if (outDeactivated) { *outDeactivated = ImGui::IsItemDeactivatedAfterEdit(); }

		return changed;
	}

	bool drawFloatSlider(const string& label, float& value, const FloatSliderData& settings, bool hideLabel, std::string format, bool* outActivated, bool* outDeactivated)
	{
		std::string l = hideLabel ? ("##" + label) : label;

		bool changed = ImGui::SliderFloat(l.c_str(), &value, settings.min, settings.max, settings.format.c_str());

		if (changed && settings.step > 0.0f) { value = round(value / settings.step) * settings.step; }
		if (outActivated) { *outActivated = ImGui::IsItemActivated(); }
		if (outDeactivated) { *outDeactivated = ImGui::IsItemDeactivatedAfterEdit(); }

		return changed;
	}

	bool drawFloatDragnSlider(const string& label, float& value, const FloatDragnSliderData& settings, bool hideLabel, 
		std::string format, bool* outActivated, bool* outDeactivated)
	{
		std::string l = hideLabel ? "##" : "";
		bool changed = false;

		float availWidth = ImGui::GetContentRegionAvail().x;
		float spacing = ImGui::GetStyle().ItemSpacing.x;
		float dragWidth = 45.0f;
		float sliderWidth = availWidth - dragWidth - spacing;

		ImGui::SetNextItemWidth(sliderWidth);
		bool sliderChanged = ImGui::SliderFloat((l + label + "_slider").c_str(), &value, settings.min, settings.max, settings.format.c_str());
		bool sliderActivated = ImGui::IsItemActivated();
		bool sliderDeactivatied = ImGui::IsItemDeactivatedAfterEdit();

		ImGui::SameLine();
		ImGui::SetNextItemWidth(dragWidth);
		bool dragChanged = ImGui::DragFloat((l + label + "_drag").c_str(), &value, settings.speed, settings.min, settings.max, settings.format.c_str());
		bool dragActivated = ImGui::IsItemActivated();
		bool dragDeactivated = ImGui::IsItemDeactivatedAfterEdit();

		changed = sliderChanged || dragChanged;
		if (changed && settings.step > 0.0f) { value = round(value / settings.step) * settings.step; }
		if (outActivated) { *outActivated = sliderActivated || dragActivated; }
		if (outDeactivated) { *outDeactivated = sliderDeactivatied || dragDeactivated; }
		
		return changed;
	}

	bool drawFloat3Color(const string& label, Math::Vec3& value, const Float3ColorData& settings, bool hideLabel, bool* outActivated, bool* outDeactivated)
	{
		std::string l = hideLabel ? ("##" + label) : label;

		bool changed = ImGui::ColorEdit3(l.c_str(), &value.x); //x부터 배열 순서대로
		if (outActivated) { *outActivated = ImGui::IsItemActivated(); }
		if (outDeactivated) { *outDeactivated = ImGui::IsItemDeactivatedAfterEdit(); }
		return changed;
	}

	bool drawFloat4Color(const string& label, Math::Vec4& value, const Float4ColorData& settings, bool hideLabel, bool* outActivated, bool* outDeactivated)
	{
		std::string l = hideLabel ? ("##" + label) : label;

		bool changed = ImGui::ColorEdit4(l.c_str(), &value.x);
		if (outActivated) { *outActivated = ImGui::IsItemActivated(); }
		if (outDeactivated) { *outDeactivated = ImGui::IsItemDeactivatedAfterEdit(); }

		return changed;
	}
}
