#version 330 core
out vec4 FragCol;

in vec3 texCoord;

uniform samplerCube skybox;

void main(){
	FragCol = texture(skybox, texCoord);
}