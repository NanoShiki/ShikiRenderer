#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec2 texCoord;
	vec3 worldPos;
	vec3 normal;
} vs_out;

layout (std140) uniform Matrices{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

void main(){
	vs_out.worldPos		= vec3(model * vec4(aPos, 1.0));
	vs_out.texCoord		= aTexCoord;
	vs_out.normal		= aNormal;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}