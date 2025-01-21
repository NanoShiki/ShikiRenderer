#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Object {
public:
	glm::vec3 rotation;
	glm::mat4 model;

	Object();
};

#endif//OBJECT_H
