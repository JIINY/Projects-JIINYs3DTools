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

	bool drawFloatDrag(const string& label, float& value, const FloatDragData& settings, bool hideLabel, std::string format)
	{
		std::string l = hideLabel ? ("##" + label) : label;
		return ImGui::DragFloat(l.c_str(), &value, settings.speed, settings.min, settings.max, settings.format.c_str());
	}

	bool drawFloatSlider(const string& label, float& value, const FloatSliderData& settings, bool hideLabel, std::string format)
	{
		std::string l = hideLabel ? ("##" + label) : label;
		bool changed = ImGui::SliderFloat(l.c_str(), &value, settings.min, settings.max, settings.format.c_str());

		if (changed && settings.step > 0.0f)
		{
			value = round(value / settings.step) * settings.step;
		}
		return changed;
	}

	bool drawFloatDragnSlider(const string& label, float& value, const FloatDragnSliderData& settings, bool hideLabel, std::string format)
	{
		std::string l = hideLabel ? "##" : "";
		bool changed = false;

		float availWidth = ImGui::GetContentRegionAvail().x;
		float spacing = ImGui::GetStyle().ItemSpacing.x;
		float dragWidth = 45.0f;
		float sliderWidth = availWidth - dragWidth - spacing;

		ImGui::SetNextItemWidth(sliderWidth);
		changed |= ImGui::SliderFloat((l + label + "_slider").c_str(), &value, settings.min, settings.max, settings.format.c_str());
		ImGui::SameLine();
		ImGui::SetNextItemWidth(dragWidth);
		changed |= ImGui::DragFloat((l + label + "_drag").c_str(), &value, settings.speed, settings.min, settings.max, settings.format.c_str());

		if (changed && settings.step > 0.0f)
		{
			value = round(value / settings.step) * settings.step;
		}
		return changed;
	}

	bool drawFloat3Color(const string& label, Math::Vec3& value, const Float3ColorData& settings, bool hideLabel)
	{
		std::string l = hideLabel ? ("##" + label) : label;
		return ImGui::ColorEdit3(l.c_str(), &value.x); //x부터 배열 순서대로
	}

	bool drawFloat4Color(const string& label, Math::Vec4& value, const Float4ColorData& settings, bool hideLabel)
	{
		std::string l = hideLabel ? ("##" + label) : label;
		return ImGui::ColorEdit4(l.c_str(), &value.x);
	}
}
