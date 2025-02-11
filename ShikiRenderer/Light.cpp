#include "Light.h"

std::vector<Light*> Light::allLights = {};

Light::Light(std::string name, LightType lightType, 
	glm::vec3 color, float aS, float dS, float sS) :
	name(name), 
	lightType(lightType), 
	color(color),
	ambientStrength(aS), 
	diffuseStrength(dS), 
	specularStrength(sS)
{
	if (lightType == DIRECTION) {
		open = true;
		direction = glm::vec3(-0.5f, -0.2f, -1.0f);
	}
	else if (lightType == POINT) {
		open = false;
		position = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else if (lightType == SPOT) {
		open = false;
		position	= RenderState::camera.Position;
		direction	= RenderState::camera.Front;
		cutoff		= 8.5f;
	}
	Light::allLights.push_back(this);
}