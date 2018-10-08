#version 330 core

layout (location = 0) in vec3 aPos;		// local space
layout (location = 1) in vec3 aNormal;	// local space

out vec3 result;

uniform vec3 lightPos;	// world space
uniform vec3 viewPos;	// world space
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // gouraud shading
    //------------------------
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
    vec3 Position = vec3(model * vec4(aPos, 1.0));
    vec3 LightPos = lightPos;

    float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - Position);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 1;
	vec3 viewDir = normalize(viewPos - Position);	
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 sepcular = specularStrength * spec * lightColor;

	result = (ambient + diffuse + sepcular) * objectColor;
}

/*

So what do we see?
You can see (for yourself or in the provided image) the clear distinction of the two triangles at the front of the 
cube. This 'stripe' is visible because of fragment interpolation. From the example image we can see that the top-right 
vertex of the cube's front face is lit with specular highlights. Since the top-right vertex of the bottom-right triangle is 
lit and the other 2 vertices of the triangle are not, the bright values interpolates to the other 2 vertices. The same 
happens for the upper-left triangle. Since the intermediate fragment colors are not directly from the light source 
but are the result of interpolation, the lighting is incorrect at the intermediate fragments and the top-left and 
bottom-right triangle collide in their brightness resulting in a visible stripe between both triangles.

This effect will become more apparent when using more complicated shapes.

*/