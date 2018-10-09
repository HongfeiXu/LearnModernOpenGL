#version 330 core

layout (location = 0) in vec3 aPos;		// local space
layout (location = 1) in vec3 aNormal;	// local space

out vec3 Normal;		// view space
out vec3 FragPos;		// view space
out vec3 LightPos;		// view space

uniform vec3 lightPos;	// world space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    LightPos = vec3(view * vec4(lightPos, 1.0));
}