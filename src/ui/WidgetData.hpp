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

	struct DisplayFloatContext
	{
		float value = 0.0f;
		float min = 0.0f;
		float max = 0.0f;
		std::string format = "%.1f";
	};

	struct DisplayFloatData
	{
		float min = 0.0f;
		float max = 0.0f;
		float step = 0.0f;
		std::string format = "%.1f";
	};

	struct DisplayIntContext
	{
		int value = 0;
		int min = 0;
		int max = 0;
		std::string format = "%d";
	};

	struct DisplayIntData
	{
		int min = 0;
		int max = 0;
		int step = 0;
	};

	struct FloatDragData
	{
		float resetValue = 0.0f;
		float speed = 0.2f;
		float min = 0.0f;
		float max = 1.0f;
		std::string format = "%.1f";
		bool useDisplayData = false;
		DisplayFloatData displayData = {};
	};

	struct FloatSliderData
	{
		float resetValue = 0.0f;
		float min = 0.0f;
		float max = 1.0f;
		float step = 0.1f;
		std::string format = "%.1f";
		bool useDisplayData = false;
		DisplayFloatData displayData = {};
	};

	struct FloatDragnSliderData
	{
		float resetValue = 0.0f;
		float speed = 0.2f;
		float min = 0.0f;
		float max = 1.0f;
		float step = 0.1f;
		std::string format = "%.1f";
		bool useDisplayData = false;
		DisplayFloatData displayData = {};
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
		DisplayFloatData, DisplayIntData,
		FloatDragData, FloatSliderData, FloatDragnSliderData,
		Float3ColorData, Float4ColorData
	>;

	float toDisplayFloat(float origin, float inMin, float inMax, const DisplayFloatData& data);
	float toOriginFloat(float display, float inMin, float inMax, const DisplayFloatData& data);
	
	void convertFloatContext(DisplayFloatContext& context, float value, const FloatDragData& settings);
	void convertFloatContext(DisplayFloatContext& context, float value, const FloatSliderData& settings);
	void convertFloatContext(DisplayFloatContext& context, float value, const FloatDragnSliderData& settings);
	void restoreFloatContext(float& value, const DisplayFloatContext& context, const FloatDragData& settings);
	void restoreFloatContext(float& value, const DisplayFloatContext& context, const FloatSliderData& settings);
	void restoreFloatContext(float& value, const DisplayFloatContext& context, const FloatDragnSliderData& settings);


	//assert 방어코드 추가할 것 + 타입 추가시 Cmd처리도 함께(e.g. CmdChangeShader, CmdEditProperties)
	bool isValidWidgetSettings(WidgetType widget, const WidgetSettings& settings);
	
	bool drawFloatDrag(const std::string& label, float& value, const FloatDragData& settings, bool hideLabel = true, 
		bool* outActivated = nullptr, bool* outDeactivated = nullptr);
	bool drawFloatSlider(const std::string& label, float& value, const FloatSliderData& settings, bool hideLabel = true, 
		bool* outActivated = nullptr, bool* outDeactivated = nullptr);
	bool drawFloatDragnSlider(const std::string& label, float& value, const FloatDragnSliderData& settings, bool hideLabel = true, 
		bool* outActivated = nullptr, bool* outDeactivated = nullptr);
	bool drawFloat3Color(const std::string& label, Math::Vec3& value, const Float3ColorData& settings, bool hideLabel = true, 
		bool* outActivated = nullptr, bool* outDeactivated = nullptr);
	bool drawFloat4Color(const std::string& label, Math::Vec4& value, const Float4ColorData& settings, bool hideLabel = true, 
		bool* outActivated = nullptr, bool* outDeactivated = nullptr);
}
