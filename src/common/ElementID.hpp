#pragma once

namespace UIElementID
{
	inline constexpr const char* Button_Mode_Cycle = "Button:Mode";

	inline constexpr const char* MainMenuBar_Group_File = "MainMenuBar:Group_File";
	inline constexpr const char* MainMenuBar_Action_Save = "MainMenuBar:Action_Save";
	inline constexpr const char* MainMenuBar_Action_Load = "MainMenuBar:Action_Load";
	inline constexpr const char* MainMenuBar_Action_Exit = "MainMenuBar:Action_Exit";

	inline constexpr const char* MainMenuBar_Group_Mode = "MainMenuBar:Group_Mode";
	inline constexpr const char* MainMenuBar_Action_Edit = "MainMenuBar:Action_Edit";
	inline constexpr const char* MainMenuBar_Action_Play = "MainMenuBar:Action_Play";

	inline constexpr const char* MainMenuBar_Group_Camera = "MainMenuBar:Group_Camera";
	inline constexpr const char* MainMenuBar_Action_FreeArm = "MainMenuBar:Action_FreeArm";
	inline constexpr const char* MainMenuBar_Action_Target = "MainMenuBar:Action_Target";
	inline constexpr const char* MainMenuBar_Action_FreeFly = "MainMenuBar:Action_FreeFly";
}


namespace CameraElementID 
{
	inline constexpr const char* EditScreen = "Camera:EditMode_Screen";
	inline constexpr const char* PlayScreen = "Camera:PlayMode_Screen";

	inline constexpr const char* EditTopView = "Camera:EditMode_TopView";
	inline constexpr const char* EditFrontView = "Camera:EditMode_FrontView";
	inline constexpr const char* EditLeftView = "Camera:EditMode_LeftView";
	inline constexpr const char* EditRightView = "Camera:EditMode_RightView";
	inline constexpr const char* EditBackView = "Camera:EditMode_BackView";
	inline constexpr const char* EditBottomView = "Camera:EditMode_BottomView";

	inline constexpr const char* ShadowRenderPass = "Camera:ShadowRenderPass";
	inline constexpr const char* DepthRenderPass = "Camera:DepthRenderPass";
	inline constexpr const char* NormalRenderPass = "Camera:NormalRenderPass";
	inline constexpr const char* LightingRenderPass = "Camera:LightingRenderPass";
	inline constexpr const char* PostProcRenderPass = "Camera:PostProcRenderPass";
}
