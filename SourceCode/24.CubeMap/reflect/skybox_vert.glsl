#version 330
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 pos = projection * view * vec4(aPos, 1.0);
    TexCoords = aPos;
    gl_Position = pos.xyww; // 将输出位置的z分量等于它的w分量，让z分量永远等于1.0，这样子的话，当透视除法执行之后，z分量会变为w / w = 1.0
}