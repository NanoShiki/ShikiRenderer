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
	static bool show_all_window = true;
	ImGui::SetNextWindowPos(ImVec2(50, 100), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, RenderState::SCREEN_HEIGHT / 2 + 250), ImGuiCond_Once);
	if (show_all_window) {
		if (ImGui::Begin("HIERARCHY")) {
			for (auto light : Light::allLights) {
				if (light->lightType == DIRECTION) {
					if (ImGui::TreeNode(light->name.c_str())) {
						ImGui::Checkbox(("Open##" + light->name).c_str(), &light->open);
						ImGui::PushItemWidth(200.0f);
						ImGui::Text("Configure:");
						ImGui::SliderFloat3(("Direction##" + light->name).c_str(), &light->direction[0], -3.0f, 3.0f);
						ImGui::SliderFloat3(("Color##" + light->name).c_str(), &light->color[0], 0.0f, 1.0f);
						ImGui::SliderFloat(("Ambient Strength##" + light->name).c_str(), &light->ambientStrength, 0.0f, 1.0f);
						ImGui::SliderFloat(("Specular Strength##" + light->name).c_str(), &light->specularStrength, 0.0f, 1.0f);
						ImGui::SliderFloat(("Diffuse Strength##" + light->name).c_str(), &light->diffuseStrength, 0.0f, 1.0f);
						ImGui::PopItemWidth();
						ImGui::TreePop();
					}
				}
				else if (light->lightType == SPOT) {
					light->position = RenderState::camera.Position;
					light->direction = RenderState::camera.Front;
					if (ImGui::TreeNode(light->name.c_str())) {
						ImGui::Checkbox(("Open##" + light->name).c_str(), &light->open);
						ImGui::PushItemWidth(200.0f);
						ImGui::Text("Configure:");
						ImGui::SliderFloat3(("Color##" + light->name).c_str(), &light->color[0], 0.0f, 1.0f);
						ImGui::SliderFloat(("Ambient Strength##" + light->name).c_str(), &light->ambientStrength, 0.0f, 1.0f);
						ImGui::SliderFloat(("Specular Strength##" + light->name).c_str(), &light->specularStrength, 0.0f, 1.0f);
						ImGui::SliderFloat(("Diffuse Strength##" + light->name).c_str(), &light->diffuseStrength, 0.0f, 1.0f);
						ImGui::SliderFloat(("Cut Off##" + light->name).c_str(), &light->cutoff, 5.0f, 60.0f);
						ImGui::PopItemWidth();
						ImGui::TreePop();
					}
				}
				else if (light->lightType == POINT) {
					if (ImGui::TreeNode("Point Light")) {
						ImGui::Checkbox(("Open##" + light->name).c_str(), &light->open);
						ImGui::PushItemWidth(200.0f);
						ImGui::Text("Configure:");
						ImGui::SliderFloat3(("Position##" + light->name).c_str(), &light->position[0], -3.0f, 3.0f);
						ImGui::SliderFloat3(("Color##" + light->name).c_str(), &light->color[0], 0.0f, 1.0f);
						ImGui::SliderFloat(("Ambient Strength##" + light->name).c_str(), &light->ambientStrength, 0.0f, 1.0f);
						ImGui::SliderFloat(("Specular Strength##" + light->name).c_str(), &light->specularStrength, 0.0f, 1.0f);
						ImGui::SliderFloat(("Diffuse Strength##" + light->name).c_str(), &light->diffuseStrength, 0.0f, 1.0f);
						ImGui::PopItemWidth();
						ImGui::TreePop();
					}
				}
			}
			for (auto object : Object::allObjects) {
				if (ImGui::TreeNode(object->name.c_str())) {
					ImGui::PushItemWidth(200.0f);
					ImGui::SeparatorText("Rotation");
					ImGui::SliderAngle(("Pitch(x-axis)##" + object->name).c_str(), &object->rotation[0], -180.0f, 180.0f);
					ImGui::SliderAngle(("Yaw(y-axis)##" + object->name).c_str(), &object->rotation[1], -180.0f, 180.0f);
					ImGui::SliderAngle(("Roll(z-axis)##" + object->name).c_str(), &object->rotation[2], -180.0f, 180.0f);
					ImGui::PopItemWidth();
					ImGui::Separator();
					ImGui::SliderFloat3(("Transform##" + object->name).c_str(), &object->position[0], -100.0f, 100.0f);
					ImGui::SliderFloat3(("Scale##" + object->name).c_str(), &object->scale[0], 0.0f, 10.0f);
					if (RenderState::enableGeometryShader && object->name != "plane") {
						ImGui::SliderFloat(("Explosion##" + object->name).c_str(), &object->explosion, 0.0f, 5.0f);
						ImGui::Checkbox(("Visualize Normal##" + object->name).c_str(), &object->visualizeNormal);
						if (object->visualizeNormal) object->explosion = 0.0f;
					}
					if (object->name == "planet") ImGui::SliderInt("Rock Amount", &Draw::rockAmount, 0, 10000);
					ImGui::TreePop();
				}
			}
		}
		ImGui::End();
		ImGui::SetNextWindowPos(ImVec2(RenderState::SCREEN_WIDTH - 550, 50), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(500, RenderState::SCREEN_HEIGHT / 1.1f), ImGuiCond_Once);
		if (ImGui::Begin("BASIC CONFIGURE")) {
			ImGui::Text("PRESS C TO ENTER CAMERA MODE");

			if (ImGui::CollapsingHeader("Renderer Configure")) {
				ImGui::ColorEdit3("Clear Color", (float*)&RenderState::clearColor);
				ImGui::Checkbox("Draw with Line", &RenderState::drawWithLine);

				ImGui::Checkbox("Enable Geometry Shader", &RenderState::enableGeometryShader);
				if (ImGui::TreeNode("Skybox")) {
					ImGui::Checkbox("Enable Skybox", &RenderState::enableSkybox);
					ImGui::BeginDisabled(!RenderState::enableSkybox);
					ImGui::RadioButton("Space", &Draw::skyboxType, Draw::SPACE);
					ImGui::RadioButton("Lake", &Draw::skyboxType, Draw::LAKE);
					ImGui::EndDisabled();
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Depth")) {
					ImGui::Checkbox("Depth Test", &RenderState::enableDepthTest);
					ImGui::BeginDisabled(!RenderState::enableDepthTest);
					ImGui::Checkbox("Show Depth Map", &RenderState::showDepthMap);
					ImGui::EndDisabled();
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Projection Mode")) {
					static int projectionMode = 1;
					ImGui::RadioButton("Perspective", &projectionMode, 1);
					ImGui::RadioButton("Orthographic", &projectionMode, 0);
					RenderState::perspective = projectionMode ? true : false;
					ImGui::BeginDisabled(RenderState::perspective);
					ImGui::PushItemWidth(200.0f);
					ImGui::SliderFloat("Orthographic Scope", &RenderState::orthoHeight, 0.0f, 10.0f);
					ImGui::PopItemWidth();
					ImGui::EndDisabled();
					ImGui::TreePop();
				}
			}
			if (ImGui::CollapsingHeader("Post-processing")) {
				ImGui::Checkbox("Use Framebuffer", &RenderState::enableFramebuffer);
				ImGui::BeginDisabled(!RenderState::enableFramebuffer);
				ImGui::RadioButton("None", &RenderState::PostProcessingCounter, -1); ImGui::SameLine();
				ImGui::RadioButton("Inversion", &RenderState::PostProcessingCounter, RenderState::INVERSION); ImGui::SameLine();
				ImGui::RadioButton("Grayscale", &RenderState::PostProcessingCounter, RenderState::GRASCALE);
				ImGui::RadioButton("Sharpen", &RenderState::PostProcessingCounter, RenderState::SHARPEN); ImGui::SameLine();
				ImGui::RadioButton("Blur", &RenderState::PostProcessingCounter, RenderState::BLUR); ImGui::SameLine();
				ImGui::RadioButton("Edge Detection", &RenderState::PostProcessingCounter, RenderState::EDGE_DETECTION);
				if (RenderState::PostProcessingCounter == -1) RenderState::enablePostProcessing = false;
				else RenderState::enablePostProcessing = true;
				ImGui::EndDisabled();
			}
		}
		ImGui::End();
	}
	{
		if (!RenderState::inCameraMode && ImGui::IsKeyPressed(ImGuiKey_C)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			RenderState::inCameraMode = true;
			show_all_window = false;
		}
		else if (RenderState::inCameraMode && ImGui::IsKeyPressed(ImGuiKey_C)) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			RenderState::inCameraMode = false;
			show_all_window = true;
		}
	}
	endFrame();
}