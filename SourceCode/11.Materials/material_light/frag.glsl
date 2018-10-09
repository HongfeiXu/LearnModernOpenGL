#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

// 物体材质属性的封装
// 需要在程序中单独设置每个属性
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

// 光照属性的封装
struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;
uniform vec3 viewPos;

void main()
{
	vec3 ambient = light.ambient * material.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse	+ specular;

	FragColor = vec4(result, 1.0);
}