#include "RenderState.h"

bool		RenderState::enableDepthTest	= true;
bool		RenderState::perspective		= true;
glm::vec4	RenderState::clearColor			= glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
float		RenderState::deltaTime			= 0.0f;
float		RenderState::lastFrame			= 0.0f;
bool		RenderState::inCameraMode		= false;
const int	RenderState::SCREEN_WIDTH		= 1980;
const int	RenderState::SCREEN_HEIGHT		= 1080;
bool		RenderState::firstMouse			= true;
float		RenderState::lastX				= RenderState::SCREEN_WIDTH / 2.0f;
float		RenderState::lastY				= RenderState::SCREEN_HEIGHT / 2.0f;
glm::mat4	RenderState::projection			= glm::mat4(1.0f);
glm::mat4	RenderState::view				= glm::mat4(1.0f);
Camera		RenderState::camera				= glm::vec3(0.0f, 0.0f, 3.0f);

void RenderState::updateFrame() {
	float currentFrame = static_cast<float>(glfwGetTime());
	RenderState::deltaTime = currentFrame - RenderState::lastFrame;
	RenderState::lastFrame = currentFrame;
}
void RenderState::updateTransform() {
	RenderState::view = glm::mat4(1.0f);
	if (RenderState::inCameraMode) RenderState::view = RenderState::camera.GetViewMatrix();
	else RenderState::view = glm::translate(RenderState::view, glm::vec3(0.0f, 0.0f, -3.0f));

	if (RenderState::perspective)
		RenderState::projection = glm::perspective(
			RenderState::inCameraMode ? glm::radians(RenderState::camera.Zoom) : glm::radians(45.0f),
			(float)RenderState::SCREEN_WIDTH / (float)RenderState::SCREEN_HEIGHT,
			0.1f, 100.0f);
	else RenderState::projection = glm::ortho(
		0.0f, (float)RenderState::SCREEN_WIDTH,
		0.0f, (float)RenderState::SCREEN_HEIGHT,
		0.1f, 100.0f);
}