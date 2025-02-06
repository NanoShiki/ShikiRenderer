#version 330 core
out vec4 fragColor;

in vec2			texCoord;
in vec3			normal;
in vec3			worldPos;

layout (std140) uniform DirLight{
	bool		DirOpen;
	float		DirAmbientStrength;
	float		DirSpecularStrength;
	float		DirDiffuseStrength;
	vec3		DirDirection;
	vec3		DirLightCol;
};

layout (std140) uniform PointLight{
	bool		PointOpen;
	float		PointAmbientStrength;
	float		PointSpecularStrength;
	float		PointDiffuseStrength;
	vec3		PointPosition;
	vec3		PointLightCol;
	
};

layout (std140) uniform SpotLight{
	bool		SpotOpen;
	float		SpotAmbientStrength;
	float		SpotSpecularStrength;
	float		SpotDiffuseStrength;
	float		SpotCutOff;
	float		SpotOuterCutOff;
	vec3		SpotPosition;
	vec3		SpotDirection;
	vec3		SpotLightCol;
	
};

struct Material{
	sampler2D	texture_diffuse1;
	sampler2D	texture_specular1;
	float		specularPow;
};
uniform			Material material;

uniform vec3	viewPos;
uniform mat3	normalMatrix;

vec3 calcDirLight(vec3 viewDir, vec3 worldNormal);
vec3 calcPointLight(vec3 viewDir, vec3 worldNormal, vec3 worldPos);
vec3 calcSpotLight(vec3 viewDir, vec3 worldNormal, vec3 worldPos);

void main(){
	vec3 viewDir			= normalize(viewPos - worldPos);
	vec3 worldNormal		= normalize(normalMatrix * normal);
	vec3 result				= vec3(0.0, 0.0, 0.0);
	if (DirOpen)
	result					+= calcDirLight(viewDir, worldNormal);
	if (PointOpen)
	result					+= calcPointLight(viewDir, worldNormal, worldPos);
	if (SpotOpen)
	result					+= calcSpotLight(viewDir, worldNormal, worldPos);
	
	fragColor				= vec4(result, 1.0);
}

vec3 calcDirLight(vec3 viewDir, vec3 worldNormal){
	vec3 halfwayVector		= normalize(-DirDirection + viewDir);
	float diff				= max(dot(worldNormal, -DirDirection), 0.0);
	float spec				= pow(max(dot(worldNormal, halfwayVector), 0.0), material.specularPow);
	vec3 ambient			= DirLightCol * DirAmbientStrength * texture(material.texture_diffuse1, texCoord).rgb;
	vec3 diffuse			= DirLightCol * DirDiffuseStrength * diff * texture(material.texture_diffuse1, texCoord).rgb;
	vec3 specular			= DirLightCol * DirSpecularStrength * spec * texture(material.texture_specular1, texCoord).rgb;
	return (ambient + diffuse + specular);
}

vec3 calcPointLight(vec3 viewDir, vec3 worldNormal, vec3 worldPos){
	vec3 lightDir			= normalize(PointPosition - worldPos);
	vec3 halfwayVector		= normalize(lightDir + viewDir);
	float diff				= max(dot(worldNormal, lightDir), 0.0);
	float spec				= pow(max(dot(worldNormal, halfwayVector), 0.0), material.specularPow);
	float constant			= 1.0;
	float linear			= 0.09;
	float quadratic			= 0.032;
	float distance			= length(PointPosition - worldPos);
	float attenuation		= 1.0 / (constant + linear * distance + quadratic * (distance * distance)); 

	vec3 ambient			= PointLightCol * PointAmbientStrength * texture(material.texture_diffuse1, texCoord).rgb;
	vec3 diffuse			= PointLightCol * PointDiffuseStrength * diff * texture(material.texture_diffuse1, texCoord).rgb;
	vec3 specular			= PointLightCol * PointSpecularStrength * spec * texture(material.texture_specular1, texCoord).rgb;

	ambient					*= attenuation;
    diffuse					*= attenuation;
    specular				*= attenuation;
	return (ambient + diffuse + specular);
}

vec3 calcSpotLight(vec3 viewDir, vec3 worldNormal, vec3 worldPos){
	vec3 lightDir			= normalize(SpotPosition - worldPos);
	vec3 halfwayVector		= normalize(lightDir + viewDir);
	float diff				= max(dot(worldNormal, lightDir), 0.0);
	float spec				= pow(max(dot(worldNormal, halfwayVector), 0.0), material.specularPow);
	float constant			= 1.0;
	float linear			= 0.09;
	float quadratic			= 0.032;
	float distance			= length(SpotPosition - worldPos);
	float attenuation		= 1.0 / (constant + linear * distance + quadratic * (distance * distance)); 

	float theta				= dot(lightDir, normalize(-SpotDirection));
	float epsilon			= SpotCutOff - SpotOuterCutOff;
	float intensity			= clamp((theta - SpotOuterCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient			= SpotLightCol * SpotAmbientStrength * texture(material.texture_diffuse1, texCoord).rgb;
	vec3 diffuse			= SpotLightCol * SpotDiffuseStrength * diff * texture(material.texture_diffuse1, texCoord).rgb;
	vec3 specular			= SpotLightCol * SpotSpecularStrength * spec * texture(material.texture_specular1, texCoord).rgb;

	ambient					*= attenuation * intensity;
    diffuse					*= attenuation * intensity;
    specular				*= attenuation * intensity;

	return (ambient + diffuse + specular);
}