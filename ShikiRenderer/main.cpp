#include "auto.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);

int main() {
	//��ʼ��glfw
	//---------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//��������(������Ƿ񴴽��ɹ�, ָ������Ϊ��ǰopengl context)
	//------------------------------------------------------
	GLFWwindow* window = glfwCreateWindow(RenderState::SCREEN_WIDTH, RenderState::SCREEN_HEIGHT, "ShikiRenderer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//��ʼ��glad, ����opengl����ָ��
	//----------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//��Ⱦ֮ǰ, ������ر����Ĵ���
	//-------------------------
	Gui gui;
	gui.initialize(window);
	Object box;
	Shader DirShader("../shader/default.vs", "../shader/directionalLight.fs");
	Shader PoiShader("../shader/default.vs", "../shader/pointLight.fs");
	Shader SpoShader("../shader/default.vs", "../shader/spotLight.fs");
	
	//��Ⱦѭ��
	//-------
	while (!glfwWindowShouldClose(window)) {
		RenderState::updateFrame();

		process_input(window);

		glClearColor(
			RenderState::clearColor.x, 
			RenderState::clearColor.y, 
			RenderState::clearColor.z, 
			RenderState::clearColor.w
		);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, box.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, box.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, box.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
		box.model = model;

		RenderState::updateTransform();
		DefaultOperation::loadTexture();
		if (RenderState::onlyDirLight) DefaultOperation::drawBox(box, DirShader, RenderState::DirectionalLight);
		if (RenderState::onlyPoiLight) DefaultOperation::drawBox(box, PoiShader, RenderState::PointLight);
		if (RenderState::onlySpoLight) DefaultOperation::drawBox(box, SpoShader, RenderState::SpotLight);

		//GUI
		gui.newFrame();
		{
			ImGui::Begin("____MENU____");

			ImGui::Text("PRESS C TO ENTER CAMERA MODE");
			if (!RenderState::inCameraMode && ImGui::IsKeyPressed(ImGuiKey_C)) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				RenderState::inCameraMode = true;
			}
			else if (RenderState::inCameraMode && ImGui::IsKeyPressed(ImGuiKey_C)) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				RenderState::inCameraMode = false;
			}

			ImGui::ColorEdit3("Clear Color", (float*)&RenderState::clearColor);

			ImGui::Checkbox("Depth Test", &RenderState::enableDepthTest);

			if (ImGui::BeginMenu("Projection Mode")) {
				if (ImGui::MenuItem("Perspective"))		RenderState::perspective = true;
				if (ImGui::MenuItem("Orthographic"))	RenderState::perspective = false;
				ImGui::EndMenu();
			}
			ImGui::PushItemWidth(200.0f);
			ImGui::SliderFloat("Orthographic Scope", &RenderState::orthoHeight, 0.0f, 10.0f);
			ImGui::Text("Rotation: ");
			ImGui::SliderAngle("Pitch(x-axis)", &box.rotation[0], -180.0f, 180.0f);
			ImGui::SliderAngle("Yaw(y-axis)",	&box.rotation[1], -180.0f, 180.0f);
			ImGui::SliderAngle("Roll(z-axis)",	&box.rotation[2], -180.0f, 180.0f);
			ImGui::PopItemWidth();

			if (ImGui::BeginMenu("Directional Light")) {
				if (ImGui::MenuItem("Only Directional Light")) {
					RenderState::onlyDirLight = true;
					RenderState::onlyPoiLight = false;
					RenderState::onlySpoLight = false;
				}
				ImGui::Text("Configure:");
				ImGui::SliderFloat3("Direction", &RenderState::dirLightDir[0], -3.0f, 3.0f);
				ImGui::SliderFloat3("Color", &RenderState::dirLightCol[0], 0.0f, 1.0f);
				ImGui::SliderFloat("Ambient Strength", &RenderState::dirAmbientStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular Strength", &RenderState::dirSpecularStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Strength", &RenderState::dirDiffuseStrength, 0.0f, 1.0f);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Point Light")) {
				if (ImGui::MenuItem("Only Directional Light")) {
					RenderState::onlyPoiLight = true;
					RenderState::onlyDirLight = false;
					RenderState::onlySpoLight = false;
				}
				ImGui::Text("Configure:");
				ImGui::SliderFloat3("Position", &RenderState::poiLightPos[0], -3.0f, 3.0f);
				ImGui::SliderFloat3("Color", &RenderState::poiLightCol[0], 0.0f, 1.0f);
				ImGui::SliderFloat("Ambient Strength", &RenderState::poiAmbientStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular Strength", &RenderState::poiSpecularStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Strength", &RenderState::poiDiffuseStrength, 0.0f, 1.0f);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Spot Light")) {
				if (ImGui::MenuItem("Only Spot Light")) {
					RenderState::onlySpoLight = true;
					RenderState::onlyPoiLight = false;
					RenderState::onlyDirLight = false;
				}
				ImGui::Text("Configure:");
				ImGui::SliderFloat3("Color", &RenderState::spoLightCol[0], 0.0f, 1.0f);
				ImGui::SliderFloat("Ambient Strength", &RenderState::spoAmbientStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular Strength", &RenderState::spoSpecularStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Strength", &RenderState::spoDiffuseStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Cut Off", &RenderState::spoCutOff, 5.0f, 60.0f);
				ImGui::EndMenu();
			}
			ImGui::End();
		}
		gui.endFrame();
		
		if (RenderState::onlyDirLight) {
			RenderState::onlyPoiLight = false;
			RenderState::onlySpoLight = false;
		}
		if (RenderState::onlyPoiLight) {
			RenderState::onlyDirLight = false;
			RenderState::onlySpoLight = false;
		}
		if (RenderState::onlySpoLight) {
			RenderState::onlyPoiLight = false;
			RenderState::onlyDirLight = false;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	gui.terminate();
	glfwTerminate();
	return 0; 
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(FORWARD, RenderState::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(BACKWARD, RenderState::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(LEFT, RenderState::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(RIGHT, RenderState::deltaTime);
}
void frame_buffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (RenderState::firstMouse) {
		RenderState::lastX = xpos;
		RenderState::lastY = ypos;
		RenderState::firstMouse = false;
	}
	float xoffset = xpos - RenderState::lastX;
	float yoffset = RenderState::lastY - ypos;
	RenderState::lastX = xpos;
	RenderState::lastY = ypos;
	RenderState::camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { RenderState::camera.ProcessMouseScroll(static_cast<float>(yoffset)); }