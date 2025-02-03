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
#include <unordered_map>
#include <vector>
#include <string>

class Draw {
public:
	static unsigned int		loadTexture(const char* path);
	static unsigned int		loadCubeMap(std::vector<std::string>& faces);
	static void				drawModel(Model& model, Object& obj, Shader& shader);
	static void				drawPlane();
	static void				setupShader(Shader& shader);
	static void				drawQuad(unsigned int& textureColorbuffer);
	static void				drawSkybox();
	static Shader*			getShader(const std::string& vPath, const std::string& fPath);

private:
	static glm::mat4		getNormalMatrix(glm::mat4& model);
	
};

#endif//DRAW_H