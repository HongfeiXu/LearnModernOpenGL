#version 330 core

out vec4 FragColor;

struct Material
{
	sampler2D diffuse;	// 漫反射贴图
	sampler2D specular;	// 高光反射贴图
	float shininess;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct FlashLight
{
	vec3 position;
	vec3 direction;
	float cutOff;		// 切光角（的余弦值）
	float outerCutOff;	// 外切光角（的余弦值）

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// attenuation
	float constant;
	float linear;
	float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform FlashLight flashLight;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 result = vec3(0,0,0);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 normal = normalize(Normal);

	result += CalcDirLight(dirLight, normal, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
	{
		result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
	}
	result += CalFlashLight(flashLight, normal, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	vec3 lightDir = light.direction;
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.specular, TexCoords).rgb);

	vec3 result = ambient + diffuse + specular;
	return result;
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.specular, TexCoords).rgb);

	float distance = length(fragPos - light.position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	return result;
}
vec3 CalFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.specular, TexCoords).rgb);


	float distance = length(fragPos - light.position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	diffuse *= attenuation;
	specular *= attenuation;

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	vec3 result =intensity * (diffuse + specular) + ambient;
	return result;
}