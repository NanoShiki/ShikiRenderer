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

	static glm::vec3	dirLightDir;			
	static glm::vec3	dirLightCol;			
	static float		dirAmbientStrength;		
	static float		dirSpecularStrength;	
	static float		dirDiffuseStrength;		
	static bool			openDirLight;			

	static glm::vec3	poiLightPos;
	static glm::vec3	poiLightCol;
	static float		poiAmbientStrength;
	static float		poiSpecularStrength;
	static float		poiDiffuseStrength;
	static bool			openPoiLight;

	static glm::vec3	spoLightCol;
	static float		spoAmbientStrength;
	static float		spoSpecularStrength;
	static float		spoDiffuseStrength;
	static float		spoCutOff;
	static bool			openSpoLight;

	static void updateFrame();					//����deltaTime��lastTime
	static void updateTransform();				//���±任����(�۲��ͶӰ)
};

#endif//RENDERSTATE_H
