#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Structs.h"

class PopupManager {
private:
	ElementInfo m_ElementInfo;
	bool m_DisplayElementInfo;
public:
	PopupManager();
	PopupManager(GLFWwindow* window);
	~PopupManager();

	void DisplayElementInfo(const ElementInfo elementInfo);
	void HideElementInfo();
	void StartOfLoop();
	void EndOfLoop();

};