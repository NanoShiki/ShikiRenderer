#ifndef GUI_H
#define GUI_H

#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_opengl3_loader.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imstb_textedit.h"
#include "ImGui/imstb_rectpack.h"
#include "ImGui/imstb_truetype.h"

class Gui {
public:
	Gui() : initialized(false) {}
	~Gui() { if (initialized) terminate(); }
	void initialize(GLFWwindow* window);
	void newFrame();
	void endFrame();
	void terminate();
private:
	bool initialized = false;

};

#endif//GUI_H
