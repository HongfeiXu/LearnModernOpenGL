#version 330 core
out vec4 FragColor;

struct Material
{
	sampler2D texture_diffuse1;	// 漫反射贴图
	sampler2D texture_specular1;	// 高光反射贴图
	float shininess;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Tangent;

uniform Material material;
uniform DirLight dirLight;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{    
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 normal = normalize(Normal);

	vec3 result = CalcDirLight(dirLight, normal, viewDir);
	
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;

	vec3 lightDir = light.direction;
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * texture(material.texture_diffuse1, TexCoords).rgb);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.texture_specular1, TexCoords).rgb);

	vec3 result = ambient + diffuse + specular;
	return result;
}