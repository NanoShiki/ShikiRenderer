#version 330 core
out vec4 fragColor;

in vec2			texCoord;
in vec3			normal;
in vec3			worldPos;

struct Light{
	vec3		position;

	vec3		lightCol;
	float		ambientStrength;
	float		pecularStrength;
	float		diffuseStrength;
};
uniform			Light light;

struct Material{
	sampler2D	diffuseMap;
	sampler2D	specularMap;
	float		specularPow;
};
uniform			Material material;

uniform vec3	viewPos;
uniform mat3	normalMatrix;

void main(){
	vec3 lightDir			= normalize(light.position - worldPos);
	vec3 viewDir			= normalize(viewPos - worldPos);
	vec3 worldNormal		= normalize(normalMatrix * normal);
	vec3 halfwayVector		= normalize(lightDir + viewDir);
	float diff				= max(dot(worldNormal, lightDir), 0.0);
	float spec				= pow(max(dot(worldNormal, halfwayVector), 0.0), material.specularPow);

	vec3 ambient			= light.ambientStrength * texture(material.diffuseMap, texCoord).rgb;
	vec3 diffuse			= light.diffuseStrength * diff * texture(material.diffuseMap, texCoord).rgb;
	vec3 specular			= light.specularStrength * spec * texture(material.specularMap, texCoord).rgb;

	vec3 result				= ambient + diffuse + specular;
	fragColor				= vec4(result, 1.0);
}