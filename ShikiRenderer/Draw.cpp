#include "Draw.h"

unsigned int Draw::uboMat				= 0;
unsigned int Draw::uboLight				= 0;

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
unsigned int Draw::loadCubeMap(std::vector<std::string>& faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
glm::mat4 Draw::getNormalMatrix(glm::mat4 mat) { return glm::mat3(glm::transpose(glm::inverse(mat))); }
void Draw::updateUniform() {
	static unsigned int uboMat = 0;
	static unsigned int uboDirLight = 0;
	static unsigned int uboPointLight = 0;
	static unsigned int uboSpotLight = 0;
	if (uboMat == 0) {
		glGenBuffers(1, &uboMat);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMat);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMat, 0, 2 * sizeof(glm::mat4));
	}
	glBindBuffer(GL_UNIFORM_BUFFER, uboMat);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(RenderState::view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(RenderState::projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	if (uboDirLight == 0) {
		glGenBuffers(1, &uboDirLight);
		glBindBuffer(GL_UNIFORM_BUFFER, uboDirLight);
		glBufferData(GL_UNIFORM_BUFFER, 48, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboDirLight, 0, 48);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, uboDirLight);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &Light::allLights[0]->open);
	glBufferSubData(GL_UNIFORM_BUFFER, 4, 4, &Light::allLights[0]->ambientStrength);
	glBufferSubData(GL_UNIFORM_BUFFER, 8, 4, &Light::allLights[0]->specularStrength);
	glBufferSubData(GL_UNIFORM_BUFFER, 12, 4, &Light::allLights[0]->diffuseStrength);
	glBufferSubData(GL_UNIFORM_BUFFER, 16, 16, &Light::allLights[0]->direction);
	glBufferSubData(GL_UNIFORM_BUFFER, 32, 16, &Light::allLights[0]->color);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if (uboPointLight == 0) {
		glGenBuffers(1, &uboPointLight);
		glBindBuffer(GL_UNIFORM_BUFFER, uboPointLight);
		glBufferData(GL_UNIFORM_BUFFER, 48, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboPointLight, 0, 48);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, uboPointLight);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &Light::allLights[1]->open);
	glBufferSubData(GL_UNIFORM_BUFFER, 4, 4, &Light::allLights[1]->ambientStrength);
	glBufferSubData(GL_UNIFORM_BUFFER, 8, 4, &Light::allLights[1]->specularStrength);
	glBufferSubData(GL_UNIFORM_BUFFER, 12, 4, &Light::allLights[1]->diffuseStrength);
	glBufferSubData(GL_UNIFORM_BUFFER, 16, 16, &Light::allLights[1]->position);
	glBufferSubData(GL_UNIFORM_BUFFER, 32, 16, &Light::allLights[1]->color);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if (uboSpotLight == 0) {
		glGenBuffers(1, &uboSpotLight);
		glBindBuffer(GL_UNIFORM_BUFFER, uboSpotLight);
		glBufferData(GL_UNIFORM_BUFFER, 80, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 3, uboSpotLight, 0, 80);
	}
	static float cutoff = 0;
	cutoff = glm::cos(glm::radians(Light::allLights[2]->cutoff));
	static float outerCutoff = 0;
	outerCutoff = glm::cos(glm::radians(Light::allLights[2]->cutoff + 5.0f));
	glBindBuffer(GL_UNIFORM_BUFFER, uboSpotLight);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &Light::allLights[2]->open);
	glBufferSubData(GL_UNIFORM_BUFFER, 4, 4, &Light::allLights[2]->ambientStrength);
	glBufferSubData(GL_UNIFORM_BUFFER, 8, 4, &Light::allLights[2]->specularStrength);
	glBufferSubData(GL_UNIFORM_BUFFER, 12, 4, &Light::allLights[2]->diffuseStrength);
	glBufferSubData(GL_UNIFORM_BUFFER, 16, 4, &cutoff);
	glBufferSubData(GL_UNIFORM_BUFFER, 20, 4, &outerCutoff);
	glBufferSubData(GL_UNIFORM_BUFFER, 32, 16, &Light::allLights[2]->position);
	glBufferSubData(GL_UNIFORM_BUFFER, 48, 16, &Light::allLights[2]->direction);
	glBufferSubData(GL_UNIFORM_BUFFER, 64, 16, &Light::allLights[2]->color);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Draw::setupShader(Shader& shader) {
	shader.use();
	if (!shader.have_been_setup) {
		unsigned int uniformBlockIndex0 = glGetUniformBlockIndex(shader.ID, "Matrices");
		glUniformBlockBinding(shader.ID, uniformBlockIndex0, 0);
		if (RenderState::haveColor && shader.have_light) {
			unsigned int uniformBlockIndex1 = glGetUniformBlockIndex(shader.ID, "DirLight");
			glUniformBlockBinding(shader.ID, uniformBlockIndex1, 1);
			unsigned int uniformBlockIndex2 = glGetUniformBlockIndex(shader.ID, "PointLight");
			glUniformBlockBinding(shader.ID, uniformBlockIndex2, 2);
			unsigned int uniformBlockIndex3 = glGetUniformBlockIndex(shader.ID, "SpotLight");
			glUniformBlockBinding(shader.ID, uniformBlockIndex3, 3);
		}
		shader.have_been_setup = true;
	}
	if (RenderState::haveColor && shader.have_light) {
		shader.setFloat("material.specularPow", 64.0f);
		shader.setVec3("viewPos", RenderState::camera.Position);
	}
	shader.setBool("enableGeometryShader", RenderState::enableGeometryShader);
}
void Draw::drawModel(Model& model, Object& obj) {
	RenderState::enableDepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	Shader* shader = NULL;
	if (RenderState::showDepthMap) shader = getShader(
		"../shader/depthMap/depthMap.vs",
		"../shader/depthMap/depthMap.fs");
	else if (RenderState::enableGeometryShader) shader = getShader(
		"../shader/" + std::string(obj.name) + "/" + std::string(obj.name) + ".vs",
		"../shader/" + std::string(obj.name) + "/" + std::string(obj.name) + ".fs",
		"../shader/" + std::string(obj.name) + "/" + std::string(obj.name) + ".gs");
	else shader = getShader(
		"../shader/" + std::string(obj.name) + "/" + std::string(obj.name) + ".vs",
		"../shader/" + std::string(obj.name) + "/" + std::string(obj.name) + ".fs");
	Draw::setupShader(*shader);
	glm::mat4 backpackModel = glm::mat4(1.0f);
	backpackModel = glm::translate(backpackModel, obj.position);
	backpackModel = glm::rotate(backpackModel, obj.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
	backpackModel = glm::rotate(backpackModel, obj.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
	backpackModel = glm::rotate(backpackModel, obj.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
	backpackModel = glm::scale(backpackModel, obj.scale);
	obj.model = backpackModel;
	shader->setMat4("model", obj.model);
	if (RenderState::haveColor) shader->setMat3("normalMatrix", Draw::getNormalMatrix(obj.model));
	if (RenderState::enableGeometryShader) shader->setFloat("explosion", obj.explosion);
	model.Draw(*shader);
	if (RenderState::enableGeometryShader && obj.visualizeNormal) Draw::visualizeNormal(model, obj);
}
void Draw::visualizeNormal(Model& model, Object& obj) {
	Shader* shader = getShader(
		"../shader/visualizeNormal/visualizeNormal.vs",
		"../shader/visualizeNormal/visualizeNormal.fs",
		"../shader/visualizeNormal/visualizeNormal.gs");
	shader->have_light = false;
	Draw::setupShader(*shader);
	shader->setMat4("model", obj.model);
	shader->setMat3("normalMatrix", Draw::getNormalMatrix(RenderState::view * obj.model));
	model.Draw(*shader);
}
void Draw::drawPlane() {
	static Object plane("plane");
	Shader* shader = NULL;
	if (RenderState::showDepthMap) shader = getShader(
		"../shader/depthMap/depthMap.vs",
		"../shader/depthMap/depthMap.fs");
	else shader = getShader(
		"../shader/" + std::string(plane.name) + "/" + std::string(plane.name) + ".vs",
		"../shader/" + std::string(plane.name) + "/" + std::string(plane.name) + ".fs");
	static unsigned int planeDiffuseMap = 0;
	if (RenderState::haveColor && planeDiffuseMap == 0)
			planeDiffuseMap = Draw::loadTexture("../resources/texture/plane/diffuse.jpg");
	Draw::setupShader(*shader);

	glm::mat4 planeModel = glm::mat4(1.0f);
	planeModel = glm::translate(planeModel, plane.position);
	planeModel = glm::rotate(planeModel, plane.rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
	planeModel = glm::rotate(planeModel, plane.rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
	planeModel = glm::rotate(planeModel, plane.rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
	planeModel = glm::scale(planeModel, plane.scale);
	plane.model = planeModel;
	shader->setMat4("model", plane.model);

	static unsigned int	planeVAO = 0, planeVBO = 0;
	if (planeVAO == 0) {
		float planeVertices[] = {
			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	if (RenderState::haveColor) {
		shader->setMat3("normalMatrix", Draw::getNormalMatrix(plane.model));
		shader->setInt("material.diffuseMap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planeDiffuseMap);
	}
	shader->setFloat("explosion", plane.explosion);

	if (RenderState::enableDepthTest) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
void Draw::drawQuad(unsigned int& textureColorbuffer) {
	Shader* shader = NULL;
	if (RenderState::enablePostProcessing) {
		if (RenderState::PostProcessingCounter == RenderState::INVERSION)
			shader = getShader("../shader/framebuffer/framebuffer.vs", "../shader/framebuffer/framebuffer_inversion.fs");
		else if (RenderState::PostProcessingCounter == RenderState::GRASCALE)
			shader = getShader("../shader/framebuffer/framebuffer.vs", "../shader/framebuffer/framebuffer_grayscale.fs");
		else if (RenderState::PostProcessingCounter == RenderState::SHARPEN)
			shader = getShader("../shader/framebuffer/framebuffer.vs", "../shader/framebuffer/framebuffer_sharpen.fs");
		else if (RenderState::PostProcessingCounter == RenderState::BLUR)
			shader = getShader("../shader/framebuffer/framebuffer.vs", "../shader/framebuffer/framebuffer_blur.fs");
		else if (RenderState::PostProcessingCounter == RenderState::EDGE_DETECTION)
			shader = getShader("../shader/framebuffer/framebuffer.vs", "../shader/framebuffer/framebuffer_edge_detection.fs");
	}
	else shader = getShader("../shader/framebuffer/framebuffer.vs", "../shader/framebuffer/framebuffer.fs");

	static unsigned int quadVAO = 0, quadVBO = 0;
	if (quadVAO == 0) {
		float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
		};
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader->use();
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
Shader* Draw::getShader(const std::string& vp, const std::string& fp) {
	static std::unordered_map<std::string, Shader> shaders;
	std::string key = vp + "|" + fp;
	if (shaders.find(key) == shaders.end()) shaders.emplace(key, Shader(vp.c_str(), fp.c_str()));
	return &shaders[key];
}
Shader* Draw::getShader(const std::string& vp, const std::string& fp, const std::string& gp) {
	static std::unordered_map<std::string, Shader> Gshaders;
	std::string key = vp + "|" + fp + "|" + gp;
	if (Gshaders.find(key) == Gshaders.end()) Gshaders.emplace(key, Shader(vp.c_str(), fp.c_str(), gp.c_str()));
	return &Gshaders[key];
}
void Draw::drawSkybox() {
	Shader* shader = getShader("../shader/skybox/skybox.vs", "../shader/skybox/skybox.fs");
	static unsigned int VAO = 0, VBO = 0;
	if (VAO == 0) {
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
	static std::vector<std::string> faces;
	if (faces.size() == 0) {
		faces.push_back("../resources/texture/skybox/right.jpg");
		faces.push_back("../resources/texture/skybox/left.jpg");
		faces.push_back("../resources/texture/skybox/top.jpg");
		faces.push_back("../resources/texture/skybox/bottom.jpg");
		faces.push_back("../resources/texture/skybox/front.jpg");
		faces.push_back("../resources/texture/skybox/back.jpg");
	}
	static unsigned int cubemap = loadCubeMap(faces);

	shader->use();
	shader->setInt("skybox", 0);

	glDepthFunc(GL_LEQUAL);
	shader->use();
	glm::mat4 view = glm::mat4(glm::mat3(RenderState::view));
	glm::mat4 projection = glm::perspective(glm::radians(RenderState::camera.Zoom), (float)RenderState::SCREEN_WIDTH / (float)RenderState::SCREEN_HEIGHT, 0.1f, 100.0f);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}
void Draw::beforeRender() {
	//����������Ⱦ����һЩ����
	glClearColor(
		RenderState::clearColor.x,
		RenderState::clearColor.y,
		RenderState::clearColor.z,
		RenderState::clearColor.w
	);
	RenderState::drawWithLine ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (RenderState::enablePostProcessing) RenderState::useFramebuffer = true;

	RenderState::updateTransform();
}
void Draw::render() {
	//����׼����Ⱦ��Ҫʹ�õı���, �����ö�Ӧ����
	//backpack data
	static Object oBackpack("backpack");
	static Model backpack("../resources/model/backpack/backpack.obj");
	//-------------
	static Light dirLight("Directional Light", DIRECTION);
	static Light PointLight("Point Light", POINT);
	static Light SpotLight("Spot Light", SPOT);

	oBackpack.position = glm::vec3(-2.0f, 2.0f, 0.0f);

	//׼����screen framebuffer
	static unsigned int framebuffer = 0;
	if (framebuffer == 0) {
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	}
	static unsigned int screenColorBuffer = 0;
	if (screenColorBuffer == 0) {
		glGenTextures(1, &screenColorBuffer);
		glBindTexture(GL_TEXTURE_2D, screenColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RenderState::SCREEN_WIDTH, RenderState::SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenColorBuffer, 0);
	}
	static unsigned int rbo = 0;
	if (rbo == 0) {
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RenderState::SCREEN_WIDTH, RenderState::SCREEN_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if (RenderState::enablePostProcessing) RenderState::useFramebuffer = true;
	RenderState::useFramebuffer ? glBindFramebuffer(GL_FRAMEBUFFER, framebuffer) : glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Draw::updateUniform();
	Draw::drawPlane();
	Draw::drawModel(backpack, oBackpack);

	if (RenderState::enableSkybox) Draw::drawSkybox();
	if (RenderState::useFramebuffer) Draw::drawQuad(screenColorBuffer);
}