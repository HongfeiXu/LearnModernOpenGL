#version 330 core

in vec3 ourColor;
in vec3 vertPos;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertPos, 1.0f);
}