#pragma once

class AppUIManager;

class AppUI {
public:
	explicit AppUI();

	void initialize(AppUIManager* manager);
	void draw();


protected:
	void drawModeButton();


private:
	AppUIManager* appUIManager_;
};
