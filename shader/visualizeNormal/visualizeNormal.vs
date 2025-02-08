#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
	vec3 normal;
} vs_out;

layout (std140) uniform Matrices{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;
uniform mat3 normalMatrix;

void main(){
    vs_out.normal		= vec3(vec4(normalMatrix * aNormal, 0.0));
	gl_Position			= view * model * vec4(aPos, 1.0);
}