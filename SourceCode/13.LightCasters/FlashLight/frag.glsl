#version 330 core

out vec4 FragColor;

struct Material
{
	sampler2D diffuse;	// 漫反射贴图
	sampler2D specular;	// 高光反射贴图
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 direction;
	float cutOff;		// 切光角（的余弦值）

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
uniform Light light;
uniform vec3 viewPos;

void main()
{

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.specular, TexCoords).rgb);


	float distance = length(FragPos - light.position);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	diffuse *= attenuation;
	specular *= attenuation;

	float theta = dot(lightDir, normalize(-light.direction));
	if(theta > light.cutOff)
	{
		vec3 result = ambient + diffuse	+ specular;
		FragColor = vec4(result, 1.0);
	}
	else
	{
		vec3 result = ambient;
		FragColor = vec4(result, 1.0);
	}
}