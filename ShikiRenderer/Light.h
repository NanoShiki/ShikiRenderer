#ifndef LIGHT_H
#define LIGHT_H

#include "RenderState.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

enum LightType{DIRECTION, POINT, SPOT};

class Light {
public:
	bool open;
	std::string name;
	glm::vec3 color;
	glm::vec3 direction;
	glm::vec3 position;
	LightType lightType;
	float cutoff;
	float ambientStrength;
	float diffuseStrength;
	float specularStrength;

	Light(std::string name, LightType lightType, 
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), 
		float aS = 0.2f, float dS = 0.8f, float sS = 1.0f);
	static std::vector<Light*> allLights;
};
#endif//LIGHT_H
