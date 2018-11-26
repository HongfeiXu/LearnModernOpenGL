#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{    
	float ratio = 1.00 / 1.52;	// 玻璃的反射率
	vec3 I = normalize(FragPos - viewPos);
	vec3 R = refract(I, normalize(Normal), ratio);
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
