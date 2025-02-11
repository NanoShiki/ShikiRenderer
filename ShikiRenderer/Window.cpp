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
	
	while (!glfwWindowShouldClose(window)) {
		//渲染器默认操作
		{
			RenderState::updateFrame();	
			if (RenderState::inCameraMode && RenderState::firstMouse) {
				RenderState::lastX = RenderState::SCREEN_WIDTH / 2.0f;
				RenderState::lastY = RenderState::SCREEN_HEIGHT / 2.0f;
				glfwSetCursorPos(this->window, RenderState::lastX, RenderState::lastY);
				RenderState::firstMouse = false;
			}
			else if (!RenderState::inCameraMode) RenderState::firstMouse = true;
			process_input(window);
		}
		//渲染
		{
			Draw::beforeRender();
			Draw::render();
		}
		gui.update(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	gui.terminate();
}
void Window::frame_buffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
void Window::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	if (RenderState::inCameraMode) {
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);
		float xoffset = xpos - RenderState::lastX;
		float yoffset = RenderState::lastY - ypos;
		RenderState::lastX = xpos;
		RenderState::lastY = ypos;
		RenderState::camera.ProcessMouseMovement(xoffset, yoffset);
	}
}
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){ 
	if (RenderState::inCameraMode)
		RenderState::camera.ProcessMouseScroll(static_cast<float>(yoffset)); 
}
void Window::process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (RenderState::inCameraMode) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(FORWARD, RenderState::deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(BACKWARD, RenderState::deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(LEFT, RenderState::deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) RenderState::camera.ProcessKeyboard(RIGHT, RenderState::deltaTime);
	}
}