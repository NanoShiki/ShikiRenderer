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
	Object box("Box");
	Object plane("Plane");
	Object oBackpack("Backpack");
	Shader boxShader("../shader/box.vs", "../shader/box.fs");
	Shader backpackShader("../shader/backpack.vs", "../shader/backpack.fs");
	Shader planeShader("../shader/plane.vs", "../shader/plane.fs");
	Shader depthMapShader("../shader/depthMap.vs", "../shader/depthMap.fs");
	Model backpack("../resources/model/backpack/backpack.obj");
	Light dirLight("Directional Light", DIRECTION);
	Light PointLight("Point Light", POINT);
	Light SpotLight("Spot Light", SPOT);
	
	box.position = glm::vec3(1.0f, 1.0f, 0.0f);
	oBackpack.position = glm::vec3(-2.0f, 1.0f, 0.0f);
	
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
			if (RenderState::drawWithLine) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		//渲染
		{
			RenderState::updateTransform();
			if (RenderState::showDepthMap) {
				Draw::drawPlane(plane, depthMapShader);
				Draw::drawBox(box, depthMapShader);
				Draw::drawBackpack(backpack, oBackpack, depthMapShader);
			}
			else {
				Draw::drawPlane(plane, planeShader);
				Draw::drawBox(box, boxShader);
				Draw::drawBackpack(backpack, oBackpack, backpackShader);

			}
		}

		gui.update(window);

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