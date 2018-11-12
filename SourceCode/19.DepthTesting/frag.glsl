#version 330 core
out vec4 FragColor;

// in vec2 TexCoords;

float near = 0.1; 
float far  = 100.0;

// uniform sampler2D texture0;

// 由深度纹理中的深度值求出在视锥体中的深度值
float LinearizeDepth(float depth) 
{
	// 《unity shader 入门精要 获取深度和法线纹理》
    return (near * far)/((near - far) * depth + far);
}

void main()
{    
	//FragColor = texture(texture0, TexCoords);
	float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far，得到0-1之间的深度值
    FragColor = vec4(vec3(depth), 1.0);
}

