#version 330 core
out vec4 fragColor;

in VS_OUT {
    vec2 texCoord;
	vec3 worldPos;
	vec3 normal;
} vs_out;

in GS_OUT {
	vec2 texCoord;
	vec3 worldPos;
	vec3 normal;
} gs_out;

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
uniform bool	enableGeometryShader;

vec3 calcDirLight(vec3 viewDir, vec3 worldNormal, vec2 texCoord);
vec3 calcPointLight(vec3 viewDir, vec3 worldNormal, vec3 worldPos, vec2 texCoord);
vec3 calcSpotLight(vec3 viewDir, vec3 worldNormal, vec3 worldPos, vec2 texCoord);

void main(){
	vec3 worldPos	= vec3(0.0, 0.0, 0.0);
	vec3 normal		= vec3(0.0, 0.0, 0.0);
	vec2 texCoord	= vec2(0.0, 0.0);
	if (enableGeometryShader) {
		worldPos	= gs_out.worldPos;
		normal		= gs_out.normal;
		texCoord	= gs_out.texCoord;
	}
	else {
		worldPos	= vs_out.worldPos;
		normal		= vs_out.normal;
		texCoord	= vs_out.texCoord;
	}
	
	vec3 viewDir			= normalize(viewPos - worldPos);
	vec3 worldNormal		= normalize(normalMatrix * normal);
	vec3 result				= vec3(0.0, 0.0, 0.0);
	if (DirOpen)
	result					+= calcDirLight(viewDir, worldNormal, texCoord);
	if (PointOpen)
	result					+= calcPointLight(viewDir, worldNormal, worldPos, texCoord);
	if (SpotOpen)
	result					+= calcSpotLight(viewDir, worldNormal, worldPos, texCoord);
	
	fragColor				= vec4(result, 1.0);
}

vec3 calcDirLight(vec3 viewDir, vec3 worldNormal, vec2 texCoord){
	vec3 halfwayVector		= normalize(-DirDirection + viewDir);
	float diff				= max(dot(worldNormal, -DirDirection), 0.0);
	float spec				= pow(max(dot(worldNormal, halfwayVector), 0.0), material.specularPow);
	vec3 ambient			= DirLightCol * DirAmbientStrength * pow(texture(material.texture_diffuse1, texCoord).rgb, vec3(2.2));
	vec3 diffuse			= DirLightCol * DirDiffuseStrength * diff * pow(texture(material.texture_diffuse1, texCoord).rgb, vec3(2.2));
	vec3 specular			= DirLightCol * DirSpecularStrength * spec * texture(material.texture_specular1, texCoord).rgb;
	return pow((ambient + diffuse + specular), vec3(1.0 / 2.2));
}

vec3 calcPointLight(vec3 viewDir, vec3 worldNormal, vec3 worldPos, vec2 texCoord){
	vec3 lightDir			= normalize(PointPosition - worldPos);
	vec3 halfwayVector		= normalize(lightDir + viewDir);
	float diff				= max(dot(worldNormal, lightDir), 0.0);
	float spec				= pow(max(dot(worldNormal, halfwayVector), 0.0), material.specularPow);
	float distance			= length(PointPosition - worldPos);
	float attenuation		= 1.0 / (distance * distance); 

	vec3 ambient			= PointLightCol * PointAmbientStrength * pow(texture(material.texture_diffuse1, texCoord).rgb, vec3(2.2));
	vec3 diffuse			= PointLightCol * PointDiffuseStrength * diff * pow(texture(material.texture_diffuse1, texCoord).rgb, vec3(2.2));
	vec3 specular			= PointLightCol * PointSpecularStrength * spec * texture(material.texture_specular1, texCoord).rgb;

	ambient					*= attenuation;
    diffuse					*= attenuation;
    specular				*= attenuation;
	return pow((ambient + diffuse + specular), vec3(1.0 / 2.2));
}

vec3 calcSpotLight(vec3 viewDir, vec3 worldNormal, vec3 worldPos, vec2 texCoord){
	vec3 lightDir			= normalize(SpotPosition - worldPos);
	vec3 halfwayVector		= normalize(lightDir + viewDir);
	float diff				= max(dot(worldNormal, lightDir), 0.0);
	float spec				= pow(max(dot(worldNormal, halfwayVector), 0.0), material.specularPow);
	float distance			= length(PointPosition - worldPos);
	float attenuation		= 1.0 / (distance * distance); 

	float theta				= dot(lightDir, normalize(-SpotDirection));
	float epsilon			= SpotCutOff - SpotOuterCutOff;
	float intensity			= clamp((theta - SpotOuterCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient			= SpotLightCol * SpotAmbientStrength * pow(texture(material.texture_diffuse1, texCoord).rgb, vec3(2.2));
	vec3 diffuse			= SpotLightCol * SpotDiffuseStrength * diff * pow(texture(material.texture_diffuse1, texCoord).rgb, vec3(2.2));
	vec3 specular			= SpotLightCol * SpotSpecularStrength * spec * texture(material.texture_specular1, texCoord).rgb;

	ambient					*= attenuation * intensity;
    diffuse					*= attenuation * intensity;
    specular				*= attenuation * intensity;

	return pow((ambient + diffuse + specular), vec3(1.0 / 2.2));
}