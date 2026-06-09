#pragma once
#include <variant>
#include "common/Math.hpp"


namespace UI
{
	enum class WidgetData
	{
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Count
	};

	enum class WidgetType
	{
		FloatDrag,
		//FloatInput,
		FloatSlider,
		FloatDragnSlider,
		Float3Color,
		Float4Color,
		Count
	};

	struct FloatDragData
	{
		float resetValue = 0.0f;
		float speed = 0.2f;
		float min = 0.0f;
		float max = 1.0f;
		std::string format = "%.1f";
	};

	struct FloatSliderData
	{
		float resetValue = 0.0f;
		float min = 0.0f;
		float max = 1.0f;
		float step = 0.1f;
		std::string format = "%.1f";
	};

	struct FloatDragnSliderData
	{
		float resetValue = 0.0f;
		float speed = 0.2f;
		float min = 0.0f;
		float max = 1.0f;
		float step = 0.1f;
		std::string format = "%.1f";
	};

	struct Float3ColorData
	{
		Math::Vec3 resetValue = { 0.0f, 0.0f, 0.0f };
	};

	struct Float4ColorData
	{
		Math::Vec4 resetValue = { 0.0f, 0.0f, 0.0f, 1.0f };
	};

	using WidgetSettings = std::variant<
		FloatDragData, FloatSliderData, FloatDragnSliderData,
		Float3ColorData, Float4ColorData
	>;

	//assert 방어코드 추가할 것
	bool isValidWidgetSettings(WidgetType widget, const WidgetSettings& settings);
	
	bool drawFloatDrag(const std::string& label, float& value, const FloatDragData& settings, bool hideLabel = true, std::string format = "%.1f");
	bool drawFloatSlider(const std::string& label, float& value, const FloatSliderData& settings, bool hideLabel = true, std::string format = "%.1f");
	bool drawFloatDragnSlider(const std::string& label, float& value, const FloatDragnSliderData& settings, bool hideLabel = true, std::string format = "%.1f");
	bool drawFloat3Color(const std::string& label, Math::Vec3& value, const Float3ColorData& settings, bool hideLabel = true);
	bool drawFloat4Color(const std::string& label, Math::Vec4& value, const Float4ColorData& settings, bool hideLabel = true);
}
