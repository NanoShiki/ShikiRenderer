#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

class RenderState {
public:
	static bool			enableDepthTest;		//是否开启深度测试
	static bool			perspective;			//是否开启透视投影
	static glm::vec4	clearColor;				//指定清理屏幕颜色
	static float		deltaTime;				//渲染间隔时间长度
	static float		lastFrame;				//上一帧渲染时间点
	static bool			inCameraMode;			//是否位于相机模式
	static const int	SCREEN_WIDTH;			//指定渲染窗口宽度
	static const int	SCREEN_HEIGHT;			//指定渲染窗口高度
	static bool			firstMouse;				//检测是否首次聚焦
	static float		lastX, lastY;			//保存上帧鼠标位置
	static glm::mat4	projection;				//设定投影变换矩阵
	static glm::mat4	view;					//设定观察变换矩阵
	static Camera		camera;					//设定当前主摄像机
	static float		orthoWidth;				//设定正交矩阵宽度
	static float		orthoHeight;			//设定正交矩阵高度

	static void updateFrame();					//更新deltaTime和lastTime
	static void updateTransform();				//更新变换矩阵(观察和投影)
};

#endif//RENDERSTATE_H
