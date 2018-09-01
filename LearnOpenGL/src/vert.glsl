#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
out vec3 vertPos;

uniform float lateralOffset;

void main()
{
    gl_Position = vec4(aPos.x + lateralOffset, -aPos.y, aPos.z, 1.0);
	ourColor = aColor;
	vertPos = gl_Position.xyz;
}