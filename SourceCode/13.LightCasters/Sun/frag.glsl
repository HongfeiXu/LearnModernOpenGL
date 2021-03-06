#version 330 core

out vec4 FragColor;

struct Material
{
	sampler2D diffuse;	// 漫反射贴图
	sampler2D specular;	// 高光反射贴图
	sampler2D emission;
	float shininess;
};

struct Light
{
	//vec3 position;
	vec3 direction;		// （光源指向物体的方向）

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(light.position - FragPos);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.specular, TexCoords).rgb);

	//vec3 emission = texture(material.emission, TexCoords).rgb;

	//vec3 result = ambient + diffuse	+ specular + emission;
	vec3 result = ambient + diffuse	+ specular;

	FragColor = vec4(result, 1.0);
}