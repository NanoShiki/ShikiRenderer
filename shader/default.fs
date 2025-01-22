#version 330 core
out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 worldPos;

uniform sampler2D	texture0;
uniform sampler2D	texture1;
uniform vec3		lightPos;
uniform vec3		lightCol;
uniform vec3		viewPos;
uniform mat3		normalMatrix;

void main(){
	fragColor				= mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.2);
	vec3 lightDir			= normalize(lightPos - worldPos);
	vec3 viewDir			= normalize(viewPos - worldPos);
	vec3 worldNormal		= normalize(normalMatrix * normal);
	float ambientStrength	= 0.2;
	float specularPow		= 32.0;
	float specularStrength	= 0.5;
	vec3 halfwayVector		= normalize(lightDir + viewDir);

	vec3 ambientLight		= lightCol * ambientStrength;
	vec3 diffuseLight		= max(dot(lightDir, worldNormal), 0.0) * lightCol;
	vec3 specularLight		= specularStrength * lightCol * pow(max(dot(halfwayVector, worldNormal), 0.0), specularPow);

	fragColor				= vec4((ambientLight + diffuseLight + specularLight) * vec3(fragColor), 1.0);
}