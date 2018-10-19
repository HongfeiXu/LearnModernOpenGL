#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Tangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    Normal = mat3(transpose(inverse(model))) * aNormal;	// 世界空间
    FragPos = vec3(model * vec4(aPos, 1.0));			// 世界空间
    TexCoords = aTexCoord;
}