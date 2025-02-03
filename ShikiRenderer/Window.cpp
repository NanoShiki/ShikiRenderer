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
	//backpack data
	Object oBackpack("Backpack");
	Shader backpackShader("../shader/backpack.vs", "../shader/backpack.fs");
	Model backpack("../resources/model/backpack/backpack.obj");
	//-------------
	Light dirLight("Directional Light", DIRECTION);
	Light PointLight("Point Light", POINT);
	Light SpotLight("Spot Light", SPOT);

	oBackpack.position = glm::vec3(-2.0f, 2.0f, 0.0f);

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RenderState::SCREEN_WIDTH, RenderState::SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RenderState::SCREEN_WIDTH, RenderState::SCREEN_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
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
			RenderState::drawWithLine? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (RenderState::inCameraMode && RenderState::firstMouse) {
				RenderState::lastX = RenderState::SCREEN_WIDTH / 2.0f;
				RenderState::lastY = RenderState::SCREEN_HEIGHT / 2.0f;
				glfwSetCursorPos(this->window, RenderState::lastX, RenderState::lastY);
				RenderState::firstMouse = false;
			}
			else if (!RenderState::inCameraMode) RenderState::firstMouse = true;
		}
		//渲染
		{
			if (RenderState::enablePostProcessing) RenderState::useFramebuffer = true;
			RenderState::useFramebuffer ? glBindFramebuffer(GL_FRAMEBUFFER, framebuffer) : glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RenderState::updateTransform();
			Draw::drawPlane();
			Draw::drawModel(backpack, oBackpack, backpackShader);
			if (RenderState::enableSkybox) Draw::drawSkybox();
			if (RenderState::useFramebuffer) Draw::drawQuad(textureColorbuffer);
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