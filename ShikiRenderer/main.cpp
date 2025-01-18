#include "auto.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main() {
	//��ʼ��glfw
	//---------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//��������(������Ƿ񴴽��ɹ�, ָ������Ϊ��ǰopengl context)
	//------------------------------------------------------
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ShikiRenderer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

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
	ImVec4 clear_color = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);

	//��Ⱦѭ��
	//-------
	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		//GUI
		gui.newFrame();
		{
			ImGui::Begin("menu");
			ImGui::ColorEdit3("clear color", (float*)&clear_color);
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

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}