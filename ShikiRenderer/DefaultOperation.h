#ifndef DEFAULTOPERATION_H
#define DEFUALTOPERATION_H

#include <glad/glad.h>
#include "Shader.h"
#include "stb_image.h"
#include "RenderState.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"

class DefaultOperation {
public:
	static void drawBox(Object& box);
};


#endif//DEFAULTOPERATION_H