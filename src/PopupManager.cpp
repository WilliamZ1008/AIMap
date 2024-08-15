#include "PopupManager.h"

PopupManager::PopupManager(){
}

PopupManager::PopupManager(GLFWwindow* window){
	m_DisplayElementInfo = false;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
}

PopupManager::~PopupManager(){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void PopupManager::DisplayElementInfo(const ElementInfo elementInfo){
	m_ElementInfo = elementInfo;
	m_DisplayElementInfo = true;
}

void PopupManager::HideElementInfo(){
	m_DisplayElementInfo = false;
}

void PopupManager::StartOfLoop(){
	// (Your code calls glfwPollEvents())
	// ...
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (m_DisplayElementInfo) {
		ImGui::Begin("Element Info");
		ImGui::Text("Element Name: %s", m_ElementInfo.en_name.c_str());
		//ImGui::Text("中文名: %s", m_ElementInfo.cn_name.c_str());
		ImGui::Text("Abbreviation: %s", m_ElementInfo.abbreviation.c_str());
		ImGui::Text("Infomation: %s", m_ElementInfo.en_induction.c_str());
		//ImGui::Text("中文释义: %s", m_ElementInfo.cn_induction.c_str());
		ImGui::End();
	}
}

void PopupManager::EndOfLoop(){
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// (Your code calls glfwSwapBuffers() etc.)
}
