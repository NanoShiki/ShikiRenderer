#ifndef DEFAULTOPERATION_H
#define DEFUALTOPERATION_H

#include "Shader.h"
#include "stb_image.h"
#include "RenderState.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"
#include "Model.h"

class DefaultOperation {
public:
	static void				drawBox(Object& box, Shader& shader);
	static void				loadBoxTexture();
	static unsigned int		loadTexture(const char* path);
	static void				drawBackpack(Model& backpack, Shader& shader);
	static void				setupShader(Shader& shader);

private:
	static unsigned int		defaultVAO, defaultVBO;
	static unsigned int		boxDiffuseMap, boxSpecularMap;
	static glm::mat4		getNormalMatrix(glm::mat4& model);
	
};

#endif//DEFAULTOPERATION_H