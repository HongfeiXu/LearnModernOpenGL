#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixValue;		// 程序控制texture1的可见度

void main()
{
	// original
//	FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);

	// exercise 1，让笑脸图片朝着另一个方向看
	vec2 TexCoord2 = vec2(-TexCoord.x, TexCoord.y);
	FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord2), mixValue);
}