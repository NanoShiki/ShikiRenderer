#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

class Object {
public:
	const char* name;
	glm::vec3	rotation;
	glm::vec3	position;
	glm::vec3	scale;
	glm::mat4	model;
	float		explosion;
	bool		visualizeNormal;
	static std::vector<Object*> allObjects;
	
	Object(const char* name);
};

#endif//OBJECT_H
