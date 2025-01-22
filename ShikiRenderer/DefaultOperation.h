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
	static bool				textureReady;

	static void drawBox(Object& box, Shader shader);
	static void loadTexture();

private:
	static unsigned int		defaultVAO, defaultVBO;
	static unsigned int		defaultTex0, defaultTex1;
	static glm::mat4		getNormalMatrix(glm::mat4& model);
};

#endif//DEFAULTOPERATION_H