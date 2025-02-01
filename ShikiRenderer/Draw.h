#ifndef DRAW_H
#define DRAW_H

#include "Shader.h"
#include "stb_image.h"
#include "RenderState.h"
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"
#include "Model.h"

class Draw {
public:
	static unsigned int		loadTexture(const char* path);
	static void				drawModel(Model& model, Object& obj, Shader& shader);
	static void				drawPlane(Object& plane, Shader& shader);
	static void				setupShader(Shader& shader);
	static void				drawQuad(Shader& shader, unsigned int& textureColorbuffer);

private:
	static unsigned int		planeVAO, planeVBO;
	static unsigned int		quadVAO, quadVBO;
	static unsigned int		planeDiffuseMap;
	static glm::mat4		getNormalMatrix(glm::mat4& model);
	
};

#endif//DRAW_H