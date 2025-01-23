#version 330 core
out vec4 fragColor;

in vec2			texCoord;
in vec3			normal;
in vec3			worldPos;

struct DirLight{
	vec3		direction;

	vec3		lightCol;
	float		ambientStrength;
	float		specularStrength;
	float		diffuseStrength;
};
uniform			DirLight dirLight;

struct Material{
	sampler2D	diffuseMap;
	sampler2D	specularMap;
	float		specularPow;
};
uniform			Material material;

uniform vec3	viewPos;
uniform mat3	normalMatrix;

void main(){
	vec3 viewDir			= normalize(viewPos - worldPos);
	vec3 worldNormal		= normalize(normalMatrix * normal);
	vec3 halfwayVector		= normalize(-dirLight.direction + viewDir);
	float diff				= max(dot(worldNormal, -dirLight.direction), 0.0);
	float spec				= pow(max(dot(worldNormal, halfwayVector), 0.0), material.specularPow);

	vec3 ambient			= dirLight.lightCol * dirLight.ambientStrength * texture(material.diffuseMap, texCoord).rgb;
	vec3 diffuse			= dirLight.lightCol * dirLight.diffuseStrength * diff * texture(material.diffuseMap, texCoord).rgb;
	vec3 specular			= dirLight.lightCol * dirLight.specularStrength * spec * texture(material.specularMap, texCoord).rgb;

	vec3 result				= ambient + diffuse + specular;
	fragColor				= vec4(result, 1.0);
}