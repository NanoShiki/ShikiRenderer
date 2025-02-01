#include "Gui.h"

const char* glsl_version = "#version 330";

Gui::Gui(GLFWwindow* window){
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("../resources/font/consolab.ttf", 20.0f);
	ImGui::GetIO().FontDefault = io.Fonts->Fonts.back();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	initialized = true;
}
Gui::~Gui() { if (initialized) terminate(); }
void Gui::newFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void Gui::endFrame() {
	if (!initialized) return;
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void Gui::terminate() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	initialized = false;
}
void Gui::update(GLFWwindow* window) {
	newFrame();
	ImGui::Begin("__BASIC CONFIGURE__");
	ImGui::Text("PRESS C TO ENTER CAMERA MODE");
	{
		if (!RenderState::inCameraMode && ImGui::IsKeyPressed(ImGuiKey_C)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			RenderState::inCameraMode = true;
		}
		else if (RenderState::inCameraMode && ImGui::IsKeyPressed(ImGuiKey_C)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			RenderState::inCameraMode = false;
		}
	}
	if (ImGui::BeginMenu("Renderer Configure")) {
		ImGui::ColorEdit3("Clear Color", (float*)&RenderState::clearColor);
		ImGui::Checkbox("Draw with Line", &RenderState::drawWithLine);
		if (ImGui::BeginMenu("Depth")) {
			ImGui::Checkbox("Depth Test", &RenderState::enableDepthTest);
			ImGui::Checkbox("Show Depth Map", &RenderState::showDepthMap);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Projection Mode")) {
			if (ImGui::MenuItem("Perspective"))		RenderState::perspective = true;
			if (ImGui::MenuItem("Orthographic"))	RenderState::perspective = false;
			ImGui::PushItemWidth(200.0f);
			ImGui::SliderFloat("Orthographic Scope", &RenderState::orthoHeight, 0.0f, 10.0f);
			ImGui::PopItemWidth();
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	for (auto light : Light::allLights) {
		if (light->lightType == DIRECTION) {
			if (ImGui::BeginMenu(light->name)) {
				ImGui::Checkbox("Open Directional Light", &light->open);
				ImGui::PushItemWidth(200.0f);
				ImGui::Text("Configure:");
				ImGui::SliderFloat3("Direction", &light->direction[0], -3.0f, 3.0f);
				ImGui::SliderFloat3("Color", &light->color[0], 0.0f, 1.0f);
				ImGui::SliderFloat("Ambient Strength", &light->ambientStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular Strength", &light->specularStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Strength", &light->diffuseStrength, 0.0f, 1.0f);
				ImGui::PopItemWidth();
				ImGui::EndMenu();
			}
		}
		else if (light->lightType == SPOT) {
			light->position = RenderState::camera.Position;
			light->direction = RenderState::camera.Front;
			if (ImGui::BeginMenu(light->name)) {
				ImGui::Checkbox("Open Spot Light", &light->open);
				ImGui::PushItemWidth(200.0f);
				ImGui::Text("Configure:");
				ImGui::SliderFloat3("Color", &light->color[0], 0.0f, 1.0f);
				ImGui::SliderFloat("Ambient Strength", &light->ambientStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular Strength", &light->specularStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Strength", &light->diffuseStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Cut Off", &light->cutoff, 5.0f, 60.0f);
				ImGui::PopItemWidth();
				ImGui::EndMenu();
			}
		}
		else if (light->lightType == POINT) {
			if (ImGui::BeginMenu("Point Light")) {
				ImGui::Checkbox("Open Point Light", &light->open);
				ImGui::PushItemWidth(200.0f);
				ImGui::Text("Configure:");
				ImGui::SliderFloat3("Position", &light->position[0], -3.0f, 3.0f);
				ImGui::SliderFloat3("Color", &light->color[0], 0.0f, 1.0f);
				ImGui::SliderFloat("Ambient Strength", &light->ambientStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular Strength", &light->specularStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Strength", &light->diffuseStrength, 0.0f, 1.0f);
				ImGui::PopItemWidth();
				ImGui::EndMenu();
			}
		}
	}
	for (auto object : Object::allObjects) {
		if (ImGui::BeginMenu(object->name)) {
			ImGui::PushItemWidth(200.0f);
			ImGui::Text("Rotation: ");
			ImGui::SliderAngle("Pitch(x-axis)", &object->rotation[0], -180.0f, 180.0f);
			ImGui::SliderAngle("Yaw(y-axis)", &object->rotation[1], -180.0f, 180.0f);
			ImGui::SliderAngle("Roll(z-axis)", &object->rotation[2], -180.0f, 180.0f);
			ImGui::PopItemWidth();
			ImGui::SliderFloat3("Transform", &object->position[0], -8.0f, 8.0f);
			ImGui::SliderFloat3("Scale", &object->scale[0], 0.0f, 5.0f);
			ImGui::EndMenu();
		}
	}
	ImGui::End();
	endFrame();
}