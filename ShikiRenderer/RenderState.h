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
	static float		lastFrame;				//��һ֡��Ⱦʱ���
	static bool			inCameraMode;			
	static const int	SCREEN_WIDTH;			
	static const int	SCREEN_HEIGHT;			
	static bool			firstMouse;				//����Ƿ��״ξ۽�
	static float		lastX, lastY;			//������֡���λ��
	static glm::mat4	projection;				
	static glm::mat4	view;					
	static Camera		camera;					
	static float		orthoHeight;			//�趨��������߶�
	static bool			showDepthMap;
	static bool			haveColor;				//�趨�Ƿ���Ⱦ��ɫ
	static bool			useFramebuffer;
	static bool			enablePostProcessing;
	static int			PostProcessingCounter;
	static bool			enableSkybox;

	static void updateFrame();					//����deltaTime��lastTime
	static void updateTransform();				//���±任����(�۲��ͶӰ)
};

#endif//RENDERSTATE_H
