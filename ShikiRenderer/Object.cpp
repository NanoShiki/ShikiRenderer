#include "Object.h"

Object::Object() :
	rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
	position(glm::vec3(0.0f, 0.0f, 0.0f)),
	scale	(glm::vec3(1.0f, 1.0f, 1.0f)),
	model	(glm::mat4(1.0f))
{}

