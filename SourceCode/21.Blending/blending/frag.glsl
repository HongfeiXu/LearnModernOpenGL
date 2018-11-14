#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

float near = 0.1; 
float far  = 100.0;

uniform sampler2D texture0;

void main()
{
	FragColor = texture(texture0, TexCoords);
}
