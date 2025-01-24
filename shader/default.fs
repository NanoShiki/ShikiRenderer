#version 330 core
out vec4 fragColor;

in vec2			texCoord;
in vec3			normal;
in vec3			worldPos;

struct DirLight{
	bool		open;
	vec3		direction;

	vec3		lightCol;
	float		ambientStrength;
	float		specularStrength;
	float		diffuseStrength;
};
uniform			DirLight dirLight;

struct PointLight{
	bool		open;
	vec3		position;

	vec3		lightCol;
	float		ambientStrength;
	float		specularStrength;
	float		diffuseStrength;
};
uniform			PointLight pointLight;

struct SpotLight{
	bool		open;
	vec3		position;
	vec3		direction;
	float		cutOff;
	float		outerCutOff;

	vec3		lightCol;
	float		ambientStrength;
	float		specularStrength;
	float		diffuseStrength;
};
uniform			SpotLight spotLight;

struct Material{
	sampler2D	diffuseMap;
	sampler2D	specularMap;
	float		specularPow;
};
uniform			Material material;

uniform vec3	viewPos;
uniform mat3	normalMatrix;

vec3 calcDirLight(DirLight dirLight, vec3 viewDir, vec3 worldNormal, vec3 halfwayVector, float diff, float spec);
vec3 calcPointLight(PointLight pointLight, vec3 viewDir, vec3 worldNormal, vec3 halfwayVector, float diff, float spec, vec3 worldPos);
vec3 calcSpotLight(SpotLight spotLight, vec3 viewDir, vec3 worldNormal, vec3 halfwayVector, float diff, float spec, vec3 worldPos);

void main(){
	vec3 viewDir			= normalize(viewPos - worldPos);
	vec3 worldNormal		= normalize(normalMatrix * normal);
	vec3 halfwayVector		= normalize(-dirLight.direction + viewDir);
	float diff				= max(dot(worldNormal, -dirLight.direction), 0.0);
	float spec				= pow(max(dot(worldNormal, halfwayVector), 0.0), material.specularPow);
	vec3 result				= vec3(0.0, 0.0, 0.0);
	if (dirLight.open)
	result					+= calcDirLight(dirLight, viewDir, worldNormal, halfwayVector, diff, spec);
	if (pointLight.open)
	result					+= calcPointLight(pointLight, viewDir, worldNormal, halfwayVector, diff, spec, worldPos);
	if (spotLight.open)
	result					+= calcSpotLight(spotLight, viewDir, worldNormal, halfwayVector, diff, spec, worldPos);
	
	fragColor				= vec4(result, 1.0);
}

vec3 calcDirLight(DirLight dirLight, vec3 viewDir, vec3 worldNormal, vec3 halfwayVector, float diff, float spec){
	vec3 ambient			= dirLight.lightCol * dirLight.ambientStrength * texture(material.diffuseMap, texCoord).rgb;
	vec3 diffuse			= dirLight.lightCol * dirLight.diffuseStrength * diff * texture(material.diffuseMap, texCoord).rgb;
	vec3 specular			= dirLight.lightCol * dirLight.specularStrength * spec * texture(material.specularMap, texCoord).rgb;
	return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight pointLight, vec3 viewDir, vec3 worldNormal, vec3 halfwayVector, float diff, float spec, vec3 worldPos){
	vec3 lightDir			= normalize(pointLight.position - worldPos);
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
	return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight spotLight, vec3 viewDir, vec3 worldNormal, vec3 halfwayVector, float diff, float spec, vec3 worldPos){
	vec3 lightDir			= normalize(spotLight.position - worldPos);
	float constant			= 1.0;
	float linear			= 0.09;
	float quadratic			= 0.032;
	float distance			= length(spotLight.position - worldPos);
	float attenuation		= 1.0 / (constant + linear * distance + quadratic * (distance * distance)); 

	float theta				= dot(lightDir, normalize(-spotLight.direction));
	float epsilon			= spotLight.cutOff - spotLight.outerCutOff;
	float intensity			= clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient			= spotLight.lightCol * spotLight.ambientStrength * texture(material.diffuseMap, texCoord).rgb;
	vec3 diffuse			= spotLight.lightCol * spotLight.diffuseStrength * diff * texture(material.diffuseMap, texCoord).rgb;
	vec3 specular			= spotLight.lightCol * spotLight.specularStrength * spec * texture(material.specularMap, texCoord).rgb;

	ambient					*= attenuation * intensity;
    diffuse					*= attenuation * intensity;
    specular				*= attenuation * intensity;

	return (ambient + diffuse + specular);
}