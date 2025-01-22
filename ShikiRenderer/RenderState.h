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
	static bool			enableDepthTest;		//�Ƿ�����Ȳ���
	static bool			perspective;			//�Ƿ���͸��ͶӰ
	static glm::vec4	clearColor;				//ָ��������Ļ��ɫ
	static float		deltaTime;				//��Ⱦ���ʱ�䳤��
	static float		lastFrame;				//��һ֡��Ⱦʱ���
	static bool			inCameraMode;			//�Ƿ�λ�����ģʽ
	static const int	SCREEN_WIDTH;			//ָ����Ⱦ���ڿ��
	static const int	SCREEN_HEIGHT;			//ָ����Ⱦ���ڸ߶�
	static bool			firstMouse;				//����Ƿ��״ξ۽�
	static float		lastX, lastY;			//������֡���λ��
	static glm::mat4	projection;				//�趨ͶӰ�任����
	static glm::mat4	view;					//�趨�۲�任����
	static Camera		camera;					//�趨��ǰ�������
	static float		orthoWidth;				//�趨����������
	static float		orthoHeight;			//�趨��������߶�

	static void updateFrame();					//����deltaTime��lastTime
	static void updateTransform();				//���±任����(�۲��ͶӰ)
};

#endif//RENDERSTATE_H
