#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
	// 将顶点属性的位置向量作为纹理的方向向量，并使用它从立方体贴图中采样纹理值。
	FragColor = texture(skybox, TexCoords);
}