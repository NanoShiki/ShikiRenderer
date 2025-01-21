#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;
	
	Shader(const char* vertexPath, const char* fragmentPath);
	inline void use() { glUseProgram(ID); }
	inline void setBool(const std::string& name, const bool& value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); }
	inline void setInt(const std::string& name, const int& value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
	inline void setFloat(const std::string& name, const float& value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
	inline void setMat4(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};


#endif //SHADER_H
