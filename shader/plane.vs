#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 worldPos;
out vec3 normal;

layout (std140) uniform Matrices{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

void main(){
	worldPos	= vec3(model * vec4(aPos, 1.0));
	texCoord	= vec2(aTexCoord.x, aTexCoord.y);
	normal		= vec3(0.0f, 1.0f, 0.0f);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}