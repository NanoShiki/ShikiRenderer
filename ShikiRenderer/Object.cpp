#include "Object.h"

std::vector<Object*> Object::allObjects = {};

Object::Object(std::string name) :
	rotation		(glm::vec3(0.0f, 0.0f, 0.0f)),
	position		(glm::vec3(0.0f, 0.0f, 0.0f)),
	scale			(glm::vec3(1.0f, 1.0f, 1.0f)),
	model			(glm::mat4(1.0f)),
	explosion		(0.0f),
	visualizeNormal	(false),
	init			(false)
{
	Object::allObjects.push_back(this);
	this->name = name;
}

