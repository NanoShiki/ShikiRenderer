#version 330 core
out vec4 fragColor;

in vec2			texCoord;
in vec3			normal;
in vec3			worldPos;

struct PointLight{
	vec3		position;

	vec3		lightCol;
	float		ambientStrength;
	float		specularStrength;
	float		diffuseStrength;
};
uniform			PointLight pointLight;

struct Material{
	sampler2D	diffuseMap;
	sampler2D	specularMap;
	float		specularPow;
};
uniform			Material material;

uniform vec3	viewPos;
uniform mat3	normalMatrix;

void main(){
	vec3 lightDir			= normalize(pointLight.position - worldPos);
	vec3 viewDir			= normalize(viewPos - worldPos);
	vec3 worldNormal		= normalize(normalMatrix * normal);
	vec3 halfwayVector		= normalize(lightDir + viewDir);
	float diff				= max(dot(worldNormal, lightDir), 0.0);
	float spec				= pow(max(dot(worldNormal, halfwayVector), 0.0), material.specularPow);
	
	float constant			= 1.0;
	float linear			= 0.09;
	float quadratic			= 0.032;
	float distance			= length(pointLight.position - worldPos);
	float attenuation		= 1.0 / (constant + linear * distance + quadratic * (distance * distance)); 

	vec3 ambient			= pointLight.lightCol * pointLight.ambientStrength * texture(material.diffuseMap, texCoord).rgb;
	vec3 diffuse			= pointLight.lightCol * pointLight.diffuseStrength * diff * texture(material.diffuseMap, texCoord).rgb;
	vec3 specular			= pointLight.lightCol * pointLight.specularStrength * spec * texture(material.specularMap, texCoord).rgb;

	ambient					*= attenuation;
    diffuse					*= attenuation;
    specular				*= attenuation;

	vec3 result				= ambient + diffuse + specular;
	fragColor				= vec4(result, 1.0);
}