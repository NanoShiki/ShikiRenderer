#include "auto.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);

int main() {
	//初始化glfw
	//---------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//创建窗口(并检测是否创建成功, 指定窗口为当前opengl context)
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

	//初始化glad, 引入opengl函数指针
	//----------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//渲染之前, 进行相关变量的创建
	//-------------------------
	Gui gui;
	gui.initialize(window);
	Object box;
	Shader defaultShader("../shader/default.vs", "../shader/default.fs");
	
	//渲染循环
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
		if (!DefaultOperation::textureReady) {
			DefaultOperation::loadTexture();
			DefaultOperation::textureReady = true;
		}
		DefaultOperation::drawBox(box, defaultShader);

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
			ImGui::SliderAngle("Orthographic Width", &RenderState::orthoWidth, 0.0f, (float)RenderState::SCREEN_WIDTH);
			ImGui::SliderAngle("Orthographic Height", &RenderState::orthoHeight, 0.0f, (float)RenderState::SCREEN_HEIGHT);

			ImGui::Text("Rotation: ");
			ImGui::SliderAngle("Pitch(x-axis)", &box.rotation[0], -180.0f, 180.0f);
			ImGui::SliderAngle("Yaw(y-axis)",	&box.rotation[1], -180.0f, 180.0f);
			ImGui::SliderAngle("Roll(z-axis)",	&box.rotation[2], -180.0f, 180.0f);

			ImGui::End();
		}
		gui.endFrame();
		
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