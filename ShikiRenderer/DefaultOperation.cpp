#include "DefaultOperation.h"

unsigned int	DefaultOperation::defaultVAO = 0;
unsigned int	DefaultOperation::defaultVBO = 0;
unsigned int	DefaultOperation::boxDiffuseMap = 0;
unsigned int	DefaultOperation::boxSpecularMap = 0;

void DefaultOperation::loadBoxTexture() {
	if (DefaultOperation::boxDiffuseMap != 0) return;
	DefaultOperation::boxDiffuseMap = DefaultOperation::loadTexture("../resources/texture/container2.png");
	DefaultOperation::boxSpecularMap = DefaultOperation::loadTexture("../resources/texture/container2_specular.png");
}
unsigned int DefaultOperation::loadTexture(const char* path)
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
void DefaultOperation::drawBox(Object& box, Shader& shader) {
	DefaultOperation::loadBoxTexture();
	DefaultOperation::setupShader(shader);

	glm::mat4 boxModel = glm::mat4(1.0f);
	boxModel = glm::translate(boxModel, box.position);
	boxModel = glm::rotate(boxModel, box.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
	boxModel = glm::rotate(boxModel, box.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
	boxModel = glm::rotate(boxModel, box.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
	boxModel = glm::scale(boxModel, box.scale);
	box.model = boxModel;
	shader.setMat4("model", box.model);
	shader.setMat3("normalMatrix", DefaultOperation::getNormalMatrix(box.model));

	if (DefaultOperation::defaultVAO == 0) {
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
		glGenVertexArrays(1, &DefaultOperation::defaultVAO);
		glGenBuffers(1, &DefaultOperation::defaultVBO);
		glBindVertexArray(DefaultOperation::defaultVAO);
		glBindBuffer(GL_ARRAY_BUFFER, DefaultOperation::defaultVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	
	shader.setInt("material.diffuseMap", 0);
	shader.setInt("material.specularMap", 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DefaultOperation::boxDiffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, DefaultOperation::boxSpecularMap);

	if (RenderState::enableDepthTest) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	glBindVertexArray(DefaultOperation::defaultVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
glm::mat4 DefaultOperation::getNormalMatrix(glm::mat4& model) { return glm::mat3(glm::transpose(glm::inverse(model))); }
void DefaultOperation::setupShader(Shader& shader) {
	shader.use();
	if (true) {
		shader.setMat4("view", RenderState::view);
		shader.setMat4("projection", RenderState::projection);

		shader.setBool("dirLight.open", RenderState::openDirLight);
		shader.setBool("pointLight.open", RenderState::openPoiLight);
		shader.setBool("spotLight.open", RenderState::openSpoLight);

		if (RenderState::openDirLight) {
			shader.setVec3("dirLight.direction", RenderState::dirLightDir);
			shader.setFloat("dirLight.ambientStrength", RenderState::dirAmbientStrength);
			shader.setFloat("dirLight.specularStrength", RenderState::dirSpecularStrength);
			shader.setFloat("dirLight.diffuseStrength", RenderState::dirDiffuseStrength);
			shader.setVec3("dirLight.lightCol", RenderState::dirLightCol);
		}
		if (RenderState::openPoiLight) {
			shader.setVec3("pointLight.position", RenderState::poiLightPos);
			shader.setFloat("pointLight.ambientStrength", RenderState::poiAmbientStrength);
			shader.setFloat("pointLight.specularStrength", RenderState::poiSpecularStrength);
			shader.setFloat("pointLight.diffuseStrength", RenderState::poiDiffuseStrength);
			shader.setVec3("pointLight.lightCol", RenderState::poiLightCol);
		}
		if (RenderState::openSpoLight) {
			shader.setVec3("spotLight.position", RenderState::camera.Position);
			shader.setVec3("spotLight.direction", RenderState::camera.Front);
			shader.setFloat("spotLight.ambientStrength", RenderState::spoAmbientStrength);
			shader.setFloat("spotLight.specularStrength", RenderState::spoSpecularStrength);
			shader.setFloat("spotLight.diffuseStrength", RenderState::spoDiffuseStrength);
			shader.setVec3("spotLight.lightCol", RenderState::spoLightCol);
			shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(RenderState::spoCutOff)));
			shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(RenderState::spoCutOff + 5.0f)));
		}

		shader.setFloat("material.specularPow", 64.0f);
		shader.setVec3("viewPos", RenderState::camera.Position);
	}
}
void DefaultOperation::drawBackpack(Model& backpack, Shader& shader) {
	DefaultOperation::setupShader(shader);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0f, -2.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setMat3("normalMatrix", DefaultOperation::getNormalMatrix(model));
	backpack.Draw(shader);
}
