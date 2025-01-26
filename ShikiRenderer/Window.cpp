#include "Window.h"

Window::Window(){
	//初始化glfw
	//---------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(RenderState::SCREEN_WIDTH, RenderState::SCREEN_HEIGHT, "ShikiRenderer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//初始化glad, 引入opengl函数指针
	//----------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
}
Window::~Window() { glfwTerminate(); }
void Window::Render() {
	Gui gui(window);
	Object box;
	Shader boxShader("../shader/box.vs", "../shader/box.fs");
	Model backpack("../resources/model/backpack/backpack.obj");
	Shader backpackShader("../shader/backpack.vs", "../shader/backpack.fs");
	box.position = glm::vec3(1.0f, 1.0f, 0.0f);
	while (!glfwWindowShouldClose(window)) {
		//渲染器默认操作
		{
			RenderState::updateFrame();
			process_input(window);
			glClearColor(
				RenderState::clearColor.x,
				RenderState::clearColor.y,
				RenderState::clearColor.z,
				RenderState::clearColor.w
			);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		//渲染
		{
			RenderState::updateTransform();
			DefaultOperation::loadBoxTexture();
			DefaultOperation::drawBox(box, boxShader);
			DefaultOperation::drawBackpack(backpack, backpackShader);
		}
		//GUI
		{
			gui.newFrame();
			ImGui::Begin("__BASIC CONFIGURE__");
			ImGui::Text("PRESS C TO ENTER CAMERA MODE");
			if (true) {
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
				ImGui::Checkbox("Depth Test", &RenderState::enableDepthTest);
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
			if (ImGui::BeginMenu("Directional Light")) {
				ImGui::Checkbox("Open Directional Light", &RenderState::openDirLight);
				ImGui::PushItemWidth(200.0f);
				ImGui::Text("Configure:");
				ImGui::SliderFloat3("Direction", &RenderState::dirLightDir[0], -3.0f, 3.0f);
				ImGui::SliderFloat3("Color", &RenderState::dirLightCol[0], 0.0f, 1.0f);
				ImGui::SliderFloat("Ambient Strength", &RenderState::dirAmbientStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular Strength", &RenderState::dirSpecularStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Strength", &RenderState::dirDiffuseStrength, 0.0f, 1.0f);
				ImGui::PopItemWidth();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Point Light")) {
				ImGui::Checkbox("Open Point Light", &RenderState::openPoiLight);
				ImGui::PushItemWidth(200.0f);
				ImGui::Text("Configure:");
				ImGui::SliderFloat3("Position", &RenderState::poiLightPos[0], -3.0f, 3.0f);
				ImGui::SliderFloat3("Color", &RenderState::poiLightCol[0], 0.0f, 1.0f);
				ImGui::SliderFloat("Ambient Strength", &RenderState::poiAmbientStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular Strength", &RenderState::poiSpecularStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Strength", &RenderState::poiDiffuseStrength, 0.0f, 1.0f);
				ImGui::PopItemWidth();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Spot Light")) {
				ImGui::Checkbox("Open Spot Light", &RenderState::openSpoLight);
				ImGui::PushItemWidth(200.0f);
				ImGui::Text("Configure:");
				ImGui::SliderFloat3("Color", &RenderState::spoLightCol[0], 0.0f, 1.0f);
				ImGui::SliderFloat("Ambient Strength", &RenderState::spoAmbientStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Specular Strength", &RenderState::spoSpecularStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Diffuse Strength", &RenderState::spoDiffuseStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("Cut Off", &RenderState::spoCutOff, 5.0f, 60.0f);
				ImGui::PopItemWidth();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Box")) {
				ImGui::PushItemWidth(200.0f);
				ImGui::Text("Rotation: ");
				ImGui::SliderAngle("Pitch(x-axis)", &box.rotation[0], -180.0f, 180.0f);
				ImGui::SliderAngle("Yaw(y-axis)", &box.rotation[1], -180.0f, 180.0f);
				ImGui::SliderAngle("Roll(z-axis)", &box.rotation[2], -180.0f, 180.0f);
				ImGui::PopItemWidth();
				ImGui::SliderFloat3("Transform", &box.position[0], -8.0f, 8.0f);
				ImGui::SliderFloat3("Scale", &box.scale[0], 0.0f, 5.0f);
				ImGui::EndMenu();
			}
			ImGui::End();
			gui.endFrame();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	gui.terminate();
}
void Window::frame_buffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
void Window::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
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
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){ 
	RenderState::camera.ProcessMouseScroll(static_cast<float>(yoffset)); 
}
void Window::process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(FORWARD, RenderState::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(BACKWARD, RenderState::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(LEFT, RenderState::deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(RIGHT, RenderState::deltaTime);
}