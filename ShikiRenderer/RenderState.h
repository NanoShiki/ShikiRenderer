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
	static enum PostProcessing{ INVERSION, GRASCALE, SHARPEN, BLUR, EDGE_DETECTION};
	static bool			drawWithLine;
	static bool			enableDepthTest;		
	static bool			perspective;			
	static glm::vec4	clearColor;				
	static float		deltaTime;				
	static float		lastFrame;				//上一帧渲染时间点
	static bool			inCameraMode;			
	static const int	SCREEN_WIDTH;			
	static const int	SCREEN_HEIGHT;			
	static bool			firstMouse;				//检测是否首次聚焦
	static float		lastX, lastY;			//保存上帧鼠标位置
	static glm::mat4	projection;				
	static glm::mat4	view;					
	static Camera		camera;					
	static float		orthoHeight;			//设定正交矩阵高度
	static bool			showDepthMap;
	static bool			haveColor;				//设定是否渲染颜色
	static bool			useFramebuffer;
	static bool			enablePostProcessing;
	static int			PostProcessingCounter;
	static bool			enableSkybox;

	static void updateFrame();					//更新deltaTime和lastTime
	static void updateTransform();				//更新变换矩阵(观察和投影)
};

#endif//RENDERSTATE_H
