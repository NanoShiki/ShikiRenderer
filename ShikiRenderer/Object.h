#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

class Object {
public:
	std::string name;
	glm::vec3	rotation;
	glm::vec3	position;
	glm::vec3	scale;
	glm::mat4	model;
	float		explosion;
	bool		visualizeNormal;
	bool		init;
	static std::vector<Object*> allObjects;
	
	Object(std::string name);
	Object() {};
};

#endif//OBJECT_H
