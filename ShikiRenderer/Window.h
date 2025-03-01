#ifndef WINDOW_H
#define WINDOW_H

#include "auto.h"
#include <string>
#include <iostream>

class Window {
public:
	Window();
	~Window();
	void Render();
	void process_input(GLFWwindow* window);
private:
	static void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	GLFWwindow* window;
};

#endif//WINDOW_H
