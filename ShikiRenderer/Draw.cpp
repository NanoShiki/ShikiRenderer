#include "Draw.h"

unsigned int	Draw::boxVAO = 0;
unsigned int	Draw::boxVBO = 0;
unsigned int	Draw::planeVAO = 0;
unsigned int	Draw::planeVBO = 0;
unsigned int	Draw::boxDiffuseMap = 0;
unsigned int	Draw::boxSpecularMap = 0;
unsigned int	Draw::planeDiffuseMap = 0;

unsigned int Draw::loadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
void Draw::drawBox(Object& box, Shader& shader) {
	if (RenderState::haveColor)
		if (Draw::boxDiffuseMap == 0) {
			Draw::boxDiffuseMap = Draw::loadTexture("../resources/texture/container2.png");
			Draw::boxSpecularMap = Draw::loadTexture("../resources/texture/container2_specular.png");
		}
	Draw::setupShader(shader);

	glm::mat4 boxModel = glm::mat4(1.0f);
	boxModel = glm::translate(boxModel, box.position);
	boxModel = glm::rotate(boxModel, box.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
	boxModel = glm::rotate(boxModel, box.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
	boxModel = glm::rotate(boxModel, box.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
	boxModel = glm::scale(boxModel, box.scale);
	box.model = boxModel;
	shader.setMat4("model", box.model);
	
	if (Draw::boxVAO == 0) {
		float boxVertices[] = {
		//vertex			//texture		//normal
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
		};
		glGenVertexArrays(1, &Draw::boxVAO);
		glGenBuffers(1, &Draw::boxVBO);
		glBindVertexArray(Draw::boxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, Draw::boxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	
	if (RenderState::haveColor) {
		shader.setMat3("normalMatrix", Draw::getNormalMatrix(box.model));
		shader.setInt("material.diffuseMap", 0);
		shader.setInt("material.specularMap", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Draw::boxDiffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Draw::boxSpecularMap);
	}
	
	if (RenderState::enableDepthTest) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	glBindVertexArray(Draw::boxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
glm::mat4 Draw::getNormalMatrix(glm::mat4& model) { return glm::mat3(glm::transpose(glm::inverse(model))); }
void Draw::setupShader(Shader& shader) {
	shader.use();

	shader.setMat4("view", RenderState::view);
	shader.setMat4("projection", RenderState::projection);
	if (RenderState::haveColor) {
		shader.setBool("dirLight.open", Light::allLights[0]->open);
		shader.setBool("pointLight.open", Light::allLights[1]->open);
		shader.setBool("spotLight.open", Light::allLights[2]->open);

		if (Light::allLights[0]->open) {
			shader.setVec3("dirLight.direction", Light::allLights[0]->direction);
			shader.setFloat("dirLight.ambientStrength", Light::allLights[0]->ambientStrength);
			shader.setFloat("dirLight.specularStrength", Light::allLights[0]->specularStrength);
			shader.setFloat("dirLight.diffuseStrength", Light::allLights[0]->diffuseStrength);
			shader.setVec3("dirLight.lightCol", Light::allLights[0]->color);
		}
		if (Light::allLights[1]->open) {
			shader.setVec3("pointLight.position", Light::allLights[1]->position);
			shader.setFloat("pointLight.ambientStrength", Light::allLights[1]->ambientStrength);
			shader.setFloat("pointLight.specularStrength", Light::allLights[1]->specularStrength);
			shader.setFloat("pointLight.diffuseStrength", Light::allLights[1]->diffuseStrength);
			shader.setVec3("pointLight.lightCol", Light::allLights[1]->color);
		}
		if (Light::allLights[2]->open) {
			shader.setVec3("spotLight.position", Light::allLights[2]->position);
			shader.setVec3("spotLight.direction", Light::allLights[2]->direction);
			shader.setFloat("spotLight.ambientStrength", Light::allLights[2]->ambientStrength);
			shader.setFloat("spotLight.specularStrength", Light::allLights[2]->specularStrength);
			shader.setFloat("spotLight.diffuseStrength", Light::allLights[2]->diffuseStrength);
			shader.setVec3("spotLight.lightCol", Light::allLights[2]->color);
			shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(Light::allLights[2]->cutoff)));
			shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(Light::allLights[2]->cutoff + 5.0f)));
		}

		shader.setFloat("material.specularPow", 64.0f);
		shader.setVec3("viewPos", RenderState::camera.Position);
	}
}
void Draw::drawBackpack(Model& backpack, Object& oBackpack, Shader& shader) {
	Draw::setupShader(shader);
	glm::mat4 backpackModel = glm::mat4(1.0f);
	backpackModel = glm::translate(backpackModel, oBackpack.position);
	backpackModel = glm::rotate(backpackModel, oBackpack.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
	backpackModel = glm::rotate(backpackModel, oBackpack.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
	backpackModel = glm::rotate(backpackModel, oBackpack.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
	backpackModel = glm::scale(backpackModel, oBackpack.scale);
	oBackpack.model = backpackModel;
	shader.setMat4("model", oBackpack.model);
	if (RenderState::haveColor) shader.setMat3("normalMatrix", Draw::getNormalMatrix(oBackpack.model));
	backpack.Draw(shader);
}
void Draw::drawPlane(Object& plane, Shader& shader) {
	if (RenderState::haveColor)
		if (Draw::planeDiffuseMap == 0) {
			Draw::planeDiffuseMap = Draw::loadTexture("../resources/texture/metal.png");
		}
	Draw::setupShader(shader);

	glm::mat4 planeModel = glm::mat4(1.0f);
	planeModel = glm::translate(planeModel, plane.position);
	planeModel = glm::rotate(planeModel, plane.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
	planeModel = glm::rotate(planeModel, plane.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
	planeModel = glm::rotate(planeModel, plane.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
	planeModel = glm::scale(planeModel, plane.scale);
	plane.model = planeModel;
	shader.setMat4("model", plane.model);

	if (Draw::planeVAO == 0) {
		float planeVertices[] = {
			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};
		glGenVertexArrays(1, &Draw::planeVAO);
		glGenBuffers(1, &Draw::planeVBO);
		glBindVertexArray(Draw::planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, Draw::planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	if (RenderState::haveColor) {
		shader.setMat3("normalMatrix", Draw::getNormalMatrix(plane.model));
		shader.setInt("material.diffuseMap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Draw::planeDiffuseMap);
	}

	if (RenderState::enableDepthTest) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	glBindVertexArray(Draw::planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}