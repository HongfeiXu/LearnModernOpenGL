# LearnOpenGL

> 想继续深入学习计算机图形学。之前有些OpenGL编程的经验，不想就此给扔了（毕竟重头开始学习DirectX也需要花些时间，倒不如先把原理弄清，在战略转移）因此，再次重拾这个系列教程，发现教程又更新了不少内容，因此更加适合温故而知新了。所以，开始吧。
>
> Date: 2018.4.19
>
> Reference Website: https://learnopengl-cn.github.io/

## ----------Getting Started----------

## Day 01 你好，窗口

> 2018.4.19

Getting started::OpenGL

Getting started::Creating a window

Getting started::Hello Window

### 1. 配置库

| 库名                                      | 作用                                           |
| ----------------------------------------- | ---------------------------------------------- |
| [GLFW](http://www.glfw.org/download.html) | 创建OpenGL上下文，定义窗口函数以及处理用户输入 |
| [GLAD](http://glad.dav1d.de/)             | 在运行时获取OpenGL函数地址                     |

我们要做的是，将头文件目录（Library\Includes）添加到工程的包含目录中，将库文件目录（Library\Libs）添加到工程的库目录中，在链接器中添加附加依赖项**opengl32.lib**，**glfw3.lib**，并且将glad.c添加到工程中。如下图。

![ProjectSetting](SourceCode/01.HelloWindow/ProjectSetting.jpg)



### 2. 输出了一个窗口！并且可以通过Esc按键关闭窗口！

![HelloWindow](SourceCode/01.HelloWindow/screenshot.png)

## Day 02 你好，三角形

> 2018.8.31

### 1. VBO,VAO,EBO

![VAO-VBO-EBO](SourceCode/02.HelloTriangle/vertex_array_objects_ebo.png)

### 2. Hello Triangle

![Hello Triangle](SourceCode/02.HelloTriangle/hello_triangle_indexed_fill.png)

## Day 03 着色器

> 2018.9.1

### GLSL

```c
#version version_number
in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

int main()
{
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  out_variable_name = weird_stuff_we_processed;
}
```

### Uniform

更新uniform值的方法，注：更新一个uniform之前你**必须**先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。 

```c
float timeValue = glfwGetTime();
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
glUseProgram(shaderProgram);
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
```

![](SourceCode/03.Shader/shaders_interpolation.gif)

### 更多属性

```c
float vertices[] = {
    // 位置              // 颜色
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};
```

因为我们添加了另一个顶点属性，并且更新了VBO的内存，我们就必须重新配置顶点属性指针。更新后的VBO内存中的数据现在看起来像这样： 

![](SourceCode/03.Shader/vertex_attribute_pointer_interleaved.png)

知道了现在使用的布局，我们就可以使用glVertexAttribPointer函数更新顶点格式， 

```c
// 位置属性
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
// 颜色属性
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
glEnableVertexAttribArray(1);
```

![](SourceCode/03.Shader/more_vertex_attributes.png)

### 自己的着色器类

练习

![](SourceCode/03.Shader/shader_class/exercise.png)

## Day 04 纹理

> 2018.9.1

- 纹理环绕方式

```c
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
```

- 纹理过滤

```c
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

- 多级渐远纹理（Mipmap）

```c
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

- 加载与创建纹理

使用 [stb_image.h](https://github.com/nothings/stb)

- 应用纹理

- 纹理单元

注：一个纹理的默认纹理单元是0，它是默认的激活纹理单元，所以教程前面部分我们没有分配一个位置值。 

```c
glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
glBindTexture(GL_TEXTURE_2D, texture);
```

- 多个纹理单元的情况：

编辑片段着色器来接收另一个采样器 

```c
#version 330 core
...

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
```

为了使用第二个纹理（以及第一个），我们必须改变一点渲染流程，先绑定两个纹理到对应的纹理单元，然后定义哪个uniform采样器对应哪个纹理单元： 

```c
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, texture1);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, texture2);

glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

使用glUniform1i设置每个采样器的方式告诉OpenGL每个着色器采样器属于哪个纹理单元。我们只需要设置一次即可，所以这个会放在渲染循环的前面 

```c
ourShader.use(); // 别忘记在设置 uniform 之前激活 shader
glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置
ourShader.setInt("texture2", 1); // 或者使用着色器类设置

while(...) 
{
    [...]
}
```

```c
stbi_set_flip_vertically_on_load(true); // stb_image.h能够在图像加载时帮助我们翻转y轴
```

![](SourceCode/04.Texture/Container/Container_Colorful.png)



![](SourceCode/04.Texture/Mix2Textures/mix.gif)

## Day 05 变换

> 2018.9.3

[GLM 库](https://github.com/g-truc/glm)

GLM库从0.9.9版本起，默认会将矩阵类型初始化为一个零矩阵（所有元素均为0），而不是单位矩阵（对角元素为1，其它元素为0）。如果你使用的是0.9.9或0.9.9以上的版本，你需要将所有的矩阵初始化改为 `glm::mat4 mat = glm::mat4(1.0f)`。如果你想与本教程的代码保持一致，请使用低于0.9.9版本的GLM，或者改用上述代码初始化所有的矩阵。 

注意：`glm::mat4` 按照列主序存储元素，则 `m[a][b]` 访问的是列序号为 a 行序号为 b 的元素。同理 `glm::mat2x4` 定义了一个 2 列 4 行的矩阵，而不是 2 行 4 列。

**这里我使用了最新的 0.9.9 版本。**

- 创建变换矩阵：（先缩放，后旋转，与阅读顺序相反）

```c
glm::mat4 trans;
trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); 
```

RS(p)

- 把矩阵传递给着色器：

```c
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}
```

```c
unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
```

- 让箱子随着时间旋转，还会重新把箱子放在窗口的右下角 

```c
glm::mat4 trans;
trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
```

在这里我们先把箱子围绕原点(0, 0, 0)旋转，之后，我们把旋转过后的箱子位移到屏幕的右下角。记住，实际的变换顺序应该与阅读顺序相反：尽管在代码中我们先位移再旋转，实际的变换却是先应用旋转再是位移的。 

TR(p)

![](SourceCode/05.Transform/rotate_0.gif)

更改构造矩阵的代码顺序，RT(p)

```c
glm::mat4 trans;
trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
```

结果如下

![](SourceCode/05.Transform/rotate_1.gif)

- 练习题

尝试调用glDrawElements画出两个箱子，**只**使用变换将其摆放在不同的位置。让这个箱子被摆放在窗口的左上角，并且会不断的缩放（而不是旋转）。（`sin`函数在这里会很有用，不过注意使用`sin`函数时应用负值会导致物体被翻转） 

![](SourceCode/05.Transform/rotate_scale.gif)

## Day 06 Coordinate Systems

> 2018.9.9



![](SourceCode/06.CoordinateSystems/coordinate_systems.png)

### 进入 3D

![](SourceCode/06.CoordinateSystems/example_0.png)

### 更多的 3D

- 使用Z缓冲，配置OpenGL来进行深度测试。

![](SourceCode/06.CoordinateSystems/example_1.png)

![](SourceCode/06.CoordinateSystems/example_2.png)

![](SourceCode/06.CoordinateSystems/example_3.gif)

## Day 07 Camera

> 2018.9.20

### 摄像机/观察空间

当我们讨论摄像机/观察空间(Camera/View Space)的时候，是在讨论以摄像机的视角作为场景原点时场景中所有的顶点坐标：观察矩阵把所有的世界坐标变换为相对于摄像机位置与方向的观察坐标。 

![](SourceCode/07.Camera/camera_axes.png)

- 摄像机位置
- 摄像机方向
- 右轴
- 上轴
- Look At 矩阵

![](SourceCode/07.Camera/lookat_matrix.png)

其中R是右向量，U是上向量，D是方向向量，P是摄像机位置向量。 

### 自由移动

WASD 控制位置变化，鼠标控制摄像头方向（可以pitch 和  yaw，不支持 roll），滚轮控制视野大小（缩放）。完成了摄像机类的编写。

## Day 08 1-7 复习

- **OpenGL**： 一个定义了函数布局和输出的图形API的正式规范。
- **GLAD**： 一个拓展加载库，用来为我们加载并设定所有OpenGL函数指针，从而让我们能够使用所有（现代）OpenGL函数。
- **视口(Viewport)**： 我们需要渲染的窗口。
- **图形管线(Graphics Pipeline)**： 一个顶点在呈现为像素之前经过的全部过程。
- **着色器(Shader)**： 一个运行在显卡上的小型程序。很多阶段的图形管道都可以使用自定义的着色器来代替原有的功能。
- **标准化设备坐标(Normalized Device Coordinates, NDC)**： 顶点在通过在剪裁坐标系中剪裁与透视除法后最终呈现在的坐标系。所有位置在NDC下-1.0到1.0的顶点将不会被丢弃并且可见。
- **顶点缓冲对象(Vertex Buffer Object)**： 一个调用显存并存储所有顶点数据供显卡使用的缓冲对象。
- **顶点数组对象(Vertex Array Object)**： 存储缓冲区和顶点属性状态。
- **索引缓冲对象(Element Buffer Object)**： 一个存储索引供索引化绘制使用的缓冲对象。
- **Uniform**： 一个特殊类型的GLSL变量。它是全局的（在一个着色器程序中每一个着色器都能够访问uniform变量），并且只需要被设定一次。
- **纹理(Texture)**： 一种包裹着物体的特殊类型图像，给物体精细的视觉效果。
- **纹理缠绕(Texture Wrapping)**： 定义了一种当纹理顶点超出范围(0, 1)时指定OpenGL如何采样纹理的模式。
- **纹理过滤(Texture Filtering)**： 定义了一种当有多种纹素选择时指定OpenGL如何采样纹理的模式。这通常在纹理被放大情况下发生。
- **多级渐远纹理(Mipmaps)**： 被存储的材质的一些缩小版本，根据距观察者的距离会使用材质的合适大小。
- **stb_image.h**： 图像加载库。
- **纹理单元(Texture Units)**： 通过绑定纹理到不同纹理单元从而允许多个纹理在同一对象上渲染。
- **向量(Vector)**： 一个定义了在空间中方向和/或位置的数学实体。
- **矩阵(Matrix)**： 一个矩形阵列的数学表达式。
- **GLM**： 一个为OpenGL打造的数学库。
- **局部空间(Local Space)**： 一个物体的初始空间。所有的坐标都是相对于物体的原点的。
- **世界空间(World Space)**： 所有的坐标都相对于全局原点。
- **观察空间(View Space)**： 所有的坐标都是从摄像机的视角观察的。
- **裁剪空间(Clip Space)**： 所有的坐标都是从摄像机视角观察的，但是该空间应用了投影。这个空间应该是一个顶点坐标最终的空间，作为顶点着色器的输出。OpenGL负责处理剩下的事情（裁剪/透视除法）。
- **屏幕空间(Screen Space)**： 所有的坐标都由屏幕视角来观察。坐标的范围是从0到屏幕的宽/高。
- **LookAt矩阵**： 一种特殊类型的观察矩阵，它创建了一个坐标系，其中所有坐标都根据从一个位置正在观察目标的用户旋转或者平移。
- **欧拉角(Euler Angles)**： 被定义为偏航角(Yaw)，俯仰角(Pitch)，和滚转角(Roll)从而允许我们通过这三个值构造任何3D方向。

## ----------Lighting----------

## Day 09 颜色

> 2018.9.27

![](SourceCode/09.Color/light_color.png)

## Day 10 基础光照

> 2018.9.30

Phong 光照模型：Ambient + Diffuse + Specular

**将模型空间发现转换到世界空间**

```c
Normal = mat3(transpose(inverse(model))) * aNormal;
```

注：

在顶点着色器中实现的冯氏光照模型叫做Gouraud着色(Gouraud Shading)，而不是冯氏着色(Phong Shading)。记住，由于插值，这种光照看起来有点逊色。冯氏着色能产生更平滑的光照效果。 



![](SourceCode/10.BasicLighting/basic_lighting_gouruad.png)

实验：

![](SourceCode/10.BasicLighting/phong.png)

### 练习

- 让光源来回移动

见 exercise_0_world_space

- 在观察空间（而不是世界空间）中计算冯氏光照

见 exercise_1_view_space

- 实现一个Gouraud着色（而不是冯氏着色）

见 exercise_2_gouraud_shading

![](SourceCode/10.BasicLighting/exercise_2.png)

## Day 11 材质

> 2018.10.9

### 材质属性

在片段着色器中，创建一个结构体来存储物体的材质属性，然后声明一个uniform变量。

```c
#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;
```

在需要的地方访问材质结构体中的属性，依据材质的颜色来计算最终的输出颜色。

```c
void main()
{    
    // 环境光
    vec3 ambient = lightColor * material.ambient;

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
```

在程序中设置适当的uniform，对物体设置材质。

```c
lightingShader.setVec3("material.ambient",  1.0f, 0.5f, 0.31f);
lightingShader.setVec3("material.diffuse",  1.0f, 0.5f, 0.31f);
lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
lightingShader.setFloat("material.shininess", 32.0f);
```

### 光的属性

```c
struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;
```

![](SourceCode/11.Materials/material_light.png)

## Day 12 光照贴图

>  2018.10.9

引入漫反射和镜面反射贴图。这允许我们对物体的漫反射分量（以及间接地对环境光分量，它们几乎总是一样的）和镜面光分量有着更精确的控制。

### 漫反射贴图

也即前面用到的纹理，通常叫做漫反射贴图（Diffuse Map）,表现了物体所有的漫反射颜色的纹理图像。另外，由于环境光颜色在几乎所有情况下都等于漫反射颜色，所以也移除了环境光材质颜色向量。

```c++
struct Material {
    sampler2D diffuse;
    vec3      specular;
    float     shininess;
}; 
...
in vec2 TexCoords;
```

接下来就是在顶点数据中添加uv坐标，以及进行漫反射贴图的读取和设置。最终得到的效果如下：

![](SourceCode/12.LightingMaps/DiffuseMap.png)

### 高光反射贴图

我们需要生成一个黑白的（如果你想得话也可以是彩色的）纹理，来定义物体每部分的镜面光强度。

镜面高光的强度可以通过图像每个像素的亮度来获取。镜面光贴图上的每个像素都可以由一个颜色向量来表示，比如说黑色代表颜色向量`vec3(0.0)`，灰色代表颜色向量`vec3(0.5)`。在片段着色器中，我们接下来会取样对应的颜色值并将它乘以光源的镜面强度。一个像素越「白」，乘积就会越大，物体的镜面光分量就会越亮。

```c
struct Material
{
	sampler2D diffuse;	// 漫反射贴图
	sampler2D specular;	// 高光反射贴图
	float shininess;
};
```

接下来就是在顶点数据中添加uv坐标，以及进行漫反射贴图的读取和设置。最终得到的效果如下：

![](SourceCode/12.LightingMaps/SpecularMap.png)

### 放射光贴图

```c
vec3 emission = texture(material.emission, TexCoords).rgb;
vec3 result = ambient + diffuse	+ specular + emission;
```

效果如下

![](SourceCode/12.LightingMaps/EmissionMap.png)

## Day 13 投光物

> 2018.10.11

### 平行光

```c
struct Light
{
	//vec3 position;
	vec3 direction;		// （光源指向物体的方向）

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
```



![](SourceCode/13.LightCasters/Sun.png)

### 点光源

衰减 Attenuation

```c
struct Light {
    vec3 position;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 实现衰减
    float constant;
    float linear;
    float quadratic;
};
```

![](SourceCode/13.LightCasters/PointLight.png)

### 聚光

手电筒 Flashlight

![](SourceCode/13.LightCasters/light_casters_spotlight_angles.png)

- `LightDir` ：从片段指向光源的向量。
- `SpotDir` ：聚光灯所指向的方向。
- `Phi` ：指定了聚光半径到的切光角。落在这个角度之外的物体都不会被这个聚光所照亮。
- `Theta` ：`LightDir` 和 `SpotDir` 向量之间的夹角。在聚光内部的话，`Theta` 应该比 `Phi` 值小。

![](SourceCode/13.LightCasters/FlashLight.png)

**平滑\软化边缘**

为了创建一种看起来边缘平滑的聚光，我们需要模拟聚光有一个内圆锥(Inner Cone)和一个外圆锥(Outer Cone)。我们可以将内圆锥设置为上一部分中的那个圆锥，但我们也需要一个外圆锥，来让光从内圆锥逐渐减暗，直到外圆锥的边界。

```c
struct Light
{
	vec3 position;
	vec3 direction;
	float cutOff;		// 切光角（的余弦值）
	float outerCutOff;	// 外圆锥的切光角（的余弦值），用来软化边缘

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// attenuation
	float constant;
	float linear;
	float quadratic;
};
```

```c
float theta     = dot(lightDir, normalize(-light.direction));
float epsilon   = light.cutOff - light.outerCutOff;
float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
...
// 将不对环境光做出影响，让它总是能有一点光
diffuse  *= intensity;
specular *= intensity;
...
```

![](SourceCode/13.LightCasters/FlashLightSoft.png)

## Day 14 多光源

> 2018.10.11

我们将模拟一个类似太阳的定向光(Directional Light)光源，四个分散在场景中的点光源(Point Light)，以及一个手电筒(Flashlight)。

为了在场景中使用多个光源，我们希望将光照计算封装到GLSL函数中。

```c
struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct FlashLight
{
	vec3 position;
	vec3 direction;
	float cutOff;		// 切光角（的余弦值）
	float outerCutOff;	// 外切光角（的余弦值）

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// attenuation
	float constant;
	float linear;
	float quadratic;
};
```

![](SourceCode/14.MultipleLights/MultipleLights.png)

## Day 15 9-14 复习

- **颜色向量(Color Vector)**：一个通过红绿蓝(RGB)分量的组合描绘大部分真实颜色的向量。一个物体的颜色实际上是该物体所不能吸收的反射颜色分量。
- **冯氏光照模型(Phong Lighting Model)**：一个通过计算环境光，漫反射，和镜面光分量的值来估计真实光照的模型。
- **环境光照(Ambient Lighting)**：通过给每个没有被光照的物体很小的亮度，使其不是完全黑暗的，从而对全局光照进行估计。
- **漫反射着色(Diffuse Shading)**：一个顶点/片段与光线方向越接近，光照会越强。使用了法向量来计算角度。
- **法向量(Normal Vector)**：一个垂直于平面的单位向量。
- **法线矩阵(Normal Matrix)**：一个3x3矩阵，或者说是没有平移的模型（或者模型-观察）矩阵。它也被以某种方式修改（逆转置），从而在应用非统一缩放时，保持法向量朝向正确的方向。否则法向量会在使用非统一缩放时被扭曲。
- **镜面光照(Specular Lighting)**：当观察者视线靠近光源在表面的反射线时会显示的镜面高光。镜面光照是由观察者的方向，光源的方向和设定高光分散量的反光度值三个量共同决定的。
- **冯氏着色(Phong Shading)**：冯氏光照模型应用在片段着色器。
- **Gouraud着色(Gouraud shading)**：冯氏光照模型应用在顶点着色器上。在使用很少数量的顶点时会产生明显的瑕疵。会得到效率提升但是损失了视觉质量。
- **GLSL结构体(GLSL struct)**：一个类似于C的结构体，用作着色器变量的容器。大部分时间用来管理输入/输出/uniform。
- **材质(Material)**：一个物体反射的环境光，漫反射，镜面光颜色。这些东西设定了物体所拥有的颜色。
- **光照属性(Light(properties))**：一个光的环境光，漫反射，镜面光的强度。可以使用任何颜色值，对每一个冯氏分量(Phong Component)定义光源发出的颜色/强度。
- **漫反射贴图(Diffuse Map)**：一个设定了每个片段中漫反射颜色的纹理图片。
- **镜面光贴图(Specular Map)**：一个设定了每一个片段的镜面光强度/颜色的纹理贴图。仅在物体的特定区域显示镜面高光。
- **定向光(Directional Light)**：只有一个方向的光源。它被建模为不管距离有多长所有光束都是平行而且其方向向量在整个场景中保持不变。
- **点光源(Point Light)**：一个在场景中有位置的，光线逐渐衰减的光源。
- **衰减(Attenuation)**：光随着距离减少强度的过程，通常使用在点光源和聚光下。
- **聚光(Spotlight)**：一个被定义为在某一个方向上的锥形的光源。
- **手电筒(Flashlight)**：一个摆放在观察者视角的聚光。
- **GLSL uniform数组(GLSL Uniform Array)**：一个uniform值数组。它的工作原理和C语言数组大致一样，只是不能动态分配内存。

## ----------Model Loading----------

## Day 16 Assimp

> 2018.10.16

**Open Asset Import Library**

当使用Assimp导入一个模型的时候，它通常会将整个模型加载进一个**场景**(Scene)对象，它会包含导入的模型/场景中的所有数据。Assimp会将场景载入为一系列的节点(Node)，每个节点包含了场景对象中所储存数据的索引，每个节点都可以有任意数量的子节点。Assimp数据结构的（简化）模型如下：

![](SourceCode/16.Assimp/assimp_structure.png)

用CMake构建Assimp.

## Day 17 Mesh

> 2018.10.16

见 mesh.h

## Day 18 Model

> 2018.10.17

见 model.h

**导入3D模型到OpenGL**

**从Assimp到网格**

**重大优化**

将所有加载过的纹理全局储存，每当我们想加载一个纹理的时候，首先去检查它有没有被加载过。如果有的话，我们会直接使用那个纹理，并跳过整个加载流程，来为我们省下很多处理能力。

**和箱子模型告别**

![](SourceCode/17.Model/Model.png)

## ----------Advanced OpenGL----------

## Day 19 深度测试

> 2018.10.19

```c
// 开启深度测试，允许深度写入，设置深度测试函数
glEnable(GL_DEPTH_TEST);
glDepthMask(GL_TRUE);
glDepthFunc(GL_LESS);

// 在每次渲染之前清除模板缓冲
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```



`gl_FragCoord` 的x和y分量代表了片段的屏幕空间坐标（其中(0, 0)位于左下角）。gl_FragCoord中也包含了一个z分量，它包含了片段真正的深度值。z值就是需要与深度缓冲内容所对比的那个值。**重要的是要记住深度缓冲中的值在屏幕空间中不是线性的（在透视矩阵应用之前在观察空间中是线性的）。**

深度缓冲中0.5的值并不代表着物体的z值是位于平截头体的中间了，这个顶点的z值实际上非常接近近平面！你可以在下图中看到z值和最终的深度缓冲值之间的非线性关系：

![](SourceCode/19.DepthTesting/depth_non_linear_graph.png)

> Ref: http://www.songho.ca/opengl/gl_projectionmatrix.html

**深度缓冲的可视化**

```c
#version 330 core
out vec4 FragColor;

float near = 0.1; 
float far  = 100.0;

// 由深度纹理中的深度值求出在视锥体中的深度值
float LinearizeDepth(float depth) 
{
	// 《unity shader 入门精要 获取深度和法线纹理》
    return (near * far)/((near - far) * depth + far);
}

void main()
{   
    // 为了演示除以 far，得到0-1之间的深度值
	float depth = LinearizeDepth(gl_FragCoord.z) / far; 
    FragColor = vec4(vec3(depth), 1.0);
}


```

![](SourceCode/19.DepthTesting/DepthTesting.png)

颜色大部分都是黑色，因为深度值的范围是0.1的**近**平面到100的**远**平面，它离我们还是非常远的。结果就是，我们相对靠近近平面，所以会得到更低的（更暗的）深度值。

**深度冲突**

一个很常见的视觉错误会在两个平面或者三角形非常紧密地平行排列在一起时会发生，深度缓冲没有足够的精度来决定两个形状哪个在前面。

深度冲突是深度缓冲的一个常见问题，当物体在远处时效果会更明显（因为深度缓冲在z值比较大的时候有着更小的精度）。深度冲突不能够被完全避免，但一般会有一些技巧有助于在你的场景中减轻或者完全避免深度冲突。

![](SourceCode/19.DepthTesting/Z-fighting.png)

第一个也是最重要的技巧是**永远不要把多个物体摆得太靠近，以至于它们的一些三角形会重叠**。

第二个技巧是**尽可能将近平面设置远一些**。

另外一个很好的技巧是牺牲一些性能，**使用更高精度的深度缓冲**。

深度冲突是一个常见的问题，但如果你组合使用了上面列举出来的技术，你可能不会再需要处理深度冲突了。

![](SourceCode/19.DepthTesting/Z-fighting_resolved.png)

使用方式一解决深度冲突问题。

## Day 20 模板测试

> 2018.11.12

模板缓冲的一个简单的例子如下：

![](SourceCode/20.StencilTesting/stencil_buffer.png)

模板缓冲首先会被清除为0，之后在模板缓冲中使用1填充了一个空心矩形。场景中的片段将会只在片段的模板值为1的时候会被渲染（其它的都被丢弃了）。


使用模板缓冲的时候你可以尽情发挥，但大体的步骤如下：

1. 启用模板缓冲的写入
2. 渲染物体，更新模板缓冲的内容
3. 禁用模板缓冲的写入
4. 渲染（其他）物体，这次根据模板缓冲的内容丢弃特定的片段



**所以，通过使用模板缓冲，我们可以根据场景中已绘制的其它物体的片段，来决定是否丢弃特定的片段。**

```c
// 启用模板测试
glEnable(GL_STENCIL_TEST);
glStencilMask(0xFF); // 每一位写入模板缓冲时都保持原样
glStencilFunc(GL_EQUAL, 1, 0xFF); // 只要一个片段的模板值等于(GL_EQUAL)参考值1，片段将会通过测试并被绘制，否则会被丢弃。

// 在每次迭代前清空模板缓冲
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
```

**模板函数**

`glStencilFunc(GLenmu func, GLint ref, GLuint mask);`

- `func`：设置模板测试函数(Stencil Test Function)。这个测试函数将会应用到已储存的模板值上和glStencilFunc函数的`ref`值上。可用的选项有：GL_NEVER、GL_LESS、GL_LEQUAL、GL_GREATER、GL_GEQUAL、GL_EQUAL、GL_NOTEQUAL和GL_ALWAYS。它们的语义和深度缓冲的函数类似。
- `ref`：设置了模板测试的参考值(Reference Value)。模板缓冲的内容将会与这个值进行比较。
- `mask`：设置一个掩码，它将会与参考值和储存的模板值在测试比较它们之前进行与(AND)运算。初始情况下所有位都为1。

`glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);`

- `sfail`：模板测试失败时采取的行为。
- `dpfail`：模板测试通过，但深度测试失败时采取的行为。
- `dppass`：模板测试和深度测试都通过时采取的行为。

每个选项都可以选用以下的其中一种行为：

| 行为         | 描述                                               |
| ------------ | -------------------------------------------------- |
| GL_KEEP      | 保持当前储存的模板值                               |
| GL_ZERO      | 将模板值设置为0                                    |
| GL_REPLACE   | 将模板值设置为glStencilFunc函数设置的`ref`值       |
| GL_INCR      | 如果模板值小于最大值则将模板值加1                  |
| GL_INCR_WRAP | 与GL_INCR一样，但如果模板值超过了最大值则归零      |
| GL_DECR      | 如果模板值大于最小值则将模板值减1                  |
| GL_DECR_WRAP | 与GL_DECR一样，但如果模板值小于0则将其设置为最大值 |
| GL_INVERT    | 按位翻转当前的模板缓冲值                           |

默认情况下`glStencilOp`是设置为`(GL_KEEP, GL_KEEP, GL_KEEP)`的，所以不论任何测试的结果是如何，模板缓冲都会保留它的值。默认的行为不会更新模板缓冲，所以如果你想写入模板缓冲的话，你需要至少对其中一个选项设置不同的值。

通过使用`glStencilFunc`和`glStencilOp`，我们可以精确地指定更新模板缓冲的时机与行为了，我们也可以指定什么时候该让模板缓冲通过，即什么时候片段需要被丢弃。

**物体轮廓**（Object Outlining）

![](SourceCode/20.StencilTesting/object_outlining_2.png)

```c
	///////////////////////////////////////////////
	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// 若模板测试通过（不管深度测试是否通过），则将模板值设置为 glStencilFunc 中的 ref 值
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

	///////////////////////////////////////////////
	// render loop
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

    glStencilMask(0x00); // 记得保证我们在绘制地板的时候不会更新模板缓冲
    normalShader.use();
    DrawFloor()  

    glStencilFunc(GL_ALWAYS, 1, 0xFF); 
    glStencilMask(0xFF); 
    DrawTwoContainers();

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); 
    glDisable(GL_DEPTH_TEST);
    shaderSingleColor.use(); 
    DrawTwoScaledUpContainers();
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
```

Further Work

你看到的物体轮廓算法在需要显示选中物体的游戏（想想策略游戏）中非常常见。这样的算法能够在一个模型类中轻松实现。你可以在模型类中设置一个boolean标记，来设置需不需要绘制边框。**如果你有创造力的话，你也可以使用后期处理滤镜(Filter)，像是高斯模糊(Gaussian Blur)，让边框看起来更自然。**

除了物体轮廓之外，模板测试还有很多用途，比如在一个后视镜中绘制纹理，让它能够绘制到镜子形状中，或者使用一个叫做阴影体积(Shadow Volume)的模板缓冲技术渲染实时阴影。模板缓冲为我们已经很丰富的OpenGL工具箱又提供了一个很好的工具。

注意：

仅仅通过缩放，来获得轮廓的方式并不能保证得到正确结果，如下

![](SourceCode/20.StencilTesting/object_outlining_false.png)

## Day 21 混合

> 2018.11.14 Blending

Alpha颜色值是颜色向量的第四个分量，你可能已经看到过它很多遍了。在这个教程之前我们都将这个第四个分量设置为1.0，让这个物体的透明度为0.0，而当alpha值为0.0时物体将会是完全透明的。

**Discard**

![](SourceCode/21.Blending/blending_no_discard.png)

![](SourceCode/21.Blending/blending_discard_a.png)

> 注意，当采样纹理的边缘的时候，OpenGL会对边缘的值和纹理下一个重复的值进行插值（因为我们将它的环绕方式设置为了GL_REPEAT。这通常是没问题的，但是由于我们使用了透明值，纹理图像的顶部将会与底部边缘的纯色值进行插值。这样的结果是一个半透明的有色边框，你可能会看见它环绕着你的纹理四边形。要想避免这个，每当你alpha纹理的时候，请将纹理的环绕方式设置为GL_CLAMP_TO_EDGE：
>
> ```
> glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
> glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
> ```

![](SourceCode/21.Blending/blending_discard_b.png)

> ref: https://stackoverflow.com/questions/8509051/is-discard-bad-for-program-performance-in-opengl
>
> It's hardware-dependent. For PowerVR hardware, and other GPUs that use tile-based rendering, using `discard` means that the TBR can no longer assume that every fragment drawn will become a pixel. This assumption is important because it allows the TBR to evaluate all the depths *first*, then only evaluate the fragment shaders for the top-most fragments. A sort of deferred rendering approach, except in hardware.

**Blending**

```c
glEnable(GL_BLEND);
glBlendFunc(GLenmu sfactor, GLenum dfactor);
glBlendColor(constantColor);	// 设置常数颜色
glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO); // 为RGB和alpha通道分别设置不同的选项
glBlendEquation(GLenum mode);	// 设置运算符
```

混合方程

```
C(result) = C(source) * F(source) + C(destination) * F(destination) 
```

- C(source)：源颜色向量。这是指源自纹理的颜色向量。
- C(destination): 目标颜色向量。这是当前存储在颜色缓冲中的颜色向量。
- F(source): 源因子值。指定了alpha值对源颜色的影响。
- F(destination): 目标因子值。指定了alpha值对目标颜色的影响。

片段着色器运行完成后，并且所有的测试都通过之后，这个混合方程(Blend Equation)才会应用到片段颜色输出与当前颜色缓冲中的值（当前片段之前储存的之前片段的颜色）上。源颜色和目标颜色将会由OpenGL自动设定，但源因子和目标因子的值可以由我们来决定。

**渲染半透明纹理**

![](SourceCode/21.Blending/blending_incorrect_order.png)

发生这一现象的原因是，深度测试和混合一起使用的话会产生一些麻烦。当写入深度缓冲时，深度缓冲不会检查片段是否是透明的，所以透明的部分会和其它值一样写入到深度缓冲中。结果就是窗户的整个四边形不论透明度都会进行深度测试。即使透明的部分应该显示背后的窗户，深度测试仍然丢弃了它们。

要想保证窗户中能够显示它们背后的窗户，我们需要首先绘制背后的这部分窗户。这也就是说在绘制的时候，我们必须先手动将窗户按照最远到最近来排序，再按照顺序渲染。

> 注意，对于草这种全透明的物体，我们可以选择丢弃透明的片段而不是混合它们，这样就解决了这些头疼的问题（没有深度问题）。

**不要打乱顺序**

要想让混合在多个物体上工作，我们需要最先绘制最远的物体，最后绘制最近的物体。普通不需要混合的物体仍然可以使用深度缓冲正常绘制，所以它们不需要排序。但我们仍要保证它们在绘制（排序的）透明物体之前已经绘制完毕了。当绘制一个有不透明和透明物体的场景的时候，大体的原则如下：

1. 先绘制所有不透明的物体。
2. 对所有透明的物体排序。
3. 按顺序绘制所有透明的物体。

联想Unity中的Render Queue。

![](SourceCode/21.Blending/blending_correct_order.png)

```c
		// 按照距离排序透明物体
		std::map<float, glm::vec3> sorted;
		for (GLuint i = 0; i < windows.size(); ++i)
		{
			float distance = glm::length(camera.Position - windows[i]);
			sorted[distance] = windows[i];
		}
		// 从远到近渲染透明物体
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
```

虽然按照距离排序物体这种方法对我们这个场景能够正常工作**，但它并没有考虑旋转、缩放或者其它的变换，奇怪形状的物体需要一个不同的计量，而不是仅仅一个位置向量。**

在场景中排序物体是一个很困难的技术，很大程度上由你场景的类型所决定，更别说它额外需要消耗的处理能力了。完整渲染一个包含不透明和透明物体的场景并不是那么容易。更高级的技术还有**次序无关透明度**(Order Independent Transparency, OIT)。

## Day 22 面剔除

> 2018.11.14
>
> Face culling

OpenGL能够检查所有面向(Front Facing)观察者的面，并渲染它们，而丢弃那些背向(Back Facing)的面，节省我们很多的片段着色器调用（它们的开销很大！）。但我们仍要告诉OpenGL哪些面是正向面(Front Face)，哪些面是背向面(Back Face)。

**环绕顺序 Winding Order**

![](SourceCode/22.FaceCulling/faceculling_windingorder.png)

```c
float vertices[] = {
    // 顺时针
    vertices[0], // 顶点1
    vertices[1], // 顶点2
    vertices[2], // 顶点3
    // 逆时针
    vertices[0], // 顶点1
    vertices[2], // 顶点3
    vertices[1]  // 顶点2  
};
```



每组组成三角形图元的三个顶点就包含了一个环绕顺序。OpenGL在渲染图元的时候将使用这个信息来决定一个三角形是一个正向三角形还是背向三角形。**默认情况下，逆时针顶点所定义的三角形将会被处理为正向三角形。**

当你定义顶点顺序的时候，你应该想象对应的三角形是面向你的，所以你定义的三角形从正面看去应该是逆时针的。这样定义顶点很棒的一点是，**实际的环绕顺序是在光栅化阶段进行的**，也就是顶点着色器运行之后。这些顶点就是从**观察者视角**所见的了。

观察者所面向的所有三角形顶点就是我们所指定的正确环绕顺序了，而立方体另一面的三角形顶点则是以相反的环绕顺序所渲染的。这样的结果就是，我们所面向的三角形将会是正向三角形，而背面的三角形则是背向三角形。下面这张图显示了这个效果：

![](SourceCode/22.FaceCulling/faceculling_frontback.png)

在顶点数据中，我们将两个三角形都以逆时针顺序定义（正面的三角形是1、2、3，背面的三角形也是1、2、3（如果我们从正面看这个三角形的话））。然而，如果从观察者当前视角使用1、2、3的顺序来绘制的话，从观察者的方向来看，背面的三角形将会是以顺时针顺序渲染的。虽然背面的三角形是以逆时针定义的，它现在是以顺时针顺序渲染的了。这正是我们想要剔除（Cull，丢弃）的不可见面了！

**面剔除**

既然已经知道如何设置顶点的环绕顺序了，我们就可以使用OpenGL的面剔除选项了，它默认是禁用状态的。要想启用面剔除，我们只需要启用OpenGL的GL_CULL_FACE选项：

```
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);
glFrontFace(GL_CCW);	// （默认）逆时针的面是正向面
```

`glCullFace(GL_BACK);`

![](SourceCode/22.FaceCulling/cullback.png)

`glCullFace(GL_FRONT);`

![](SourceCode/22.FaceCulling/cullfront.png)

## Day 23 帧缓冲

> 2018.11.25 Framebuffers

### 创建一个帧缓冲

```c
unsigned int fbo;
glGenFramebuffers(1. &fbo);
glBindFramebuffer(GL_FRAMEBUFFER, fbo);

// 为帧缓冲创建附件，并将附件附加到帧缓冲上
// ...

if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMMERBUFFER_COMPLETE)
	// 帧缓冲是完整的，处理帧缓冲

```

一个完整的帧缓冲需要满足以下的条件：

- 附加至少一个缓冲（颜色、深度或模板缓冲）。
- 至少有一个颜色**附件**(Attachment)。
- 所有的附件都必须是完整的（保留了内存）。
- 每个缓冲都应该有相同的样本数。

由于我们的帧缓冲不是默认帧缓冲，渲染指令将不会对窗口的视觉输出有任何影响。出于这个原因，渲染到一个不同的帧缓冲被叫做**离屏渲染(Off-screen Rendering)**。要保证所有的渲染操作在主窗口中有视觉效果，我们需要再次激活默认帧缓冲，将它绑定到`0`。

```c
glBindFramebuffer(GL_FRAMEBUFFER, 0);

// 在完成所有帧缓冲操作后，删除这个帧缓冲对象
glDeleteFramebuffers(1, &fbo);
```

在完整性检查执行之前，我们需要给帧缓冲附加一个附件。附件是一个内存位置，它能够作为帧缓冲的一个缓冲，可以将它想象为一个图像。当创建一个附件的时候我们有两个选项：**纹理或渲染缓冲对象**(Renderbuffer Object)。

### 纹理附件

当把一个纹理附加到帧缓冲的时候，所有的渲染指令将会写入到这个纹理中，就想它是一个普通的颜色/深度或模板缓冲一样。使用纹理的优点是，所有渲染操作的结果将会被储存在一个纹理图像中，我们之后可以在着色器中很方便地使用它。

为帧缓冲创建一个纹理和创建一个普通的纹理差不多：

```c
unsigned int texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

主要的区别就是，我们将维度设置为了屏幕大小（尽管这不是必须的），并且我们给纹理的`data`参数传递了`NULL`。对于这个纹理，我们仅仅分配了内存而没有填充它。填充这个纹理将会在我们渲染到帧缓冲之后来进行。同样注意我们并不关心环绕方式或多级渐远纹理，我们在大多数情况下都不会需要它们。

现在我们已经创建好一个纹理了，要做的最后一件事就是将它**附加到帧缓冲**上了：

```c
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
```

- target：帧缓冲的目标（绘制、读取或者两者皆有）
- attachment：我们想要附加的附件类型。当前我们正在附加一个颜色附件。注意最后的0意味着我们可以附加多个颜色附件。我们将在之后的教程中提到。
- textarget：你希望附加的纹理类型
- texture：要附加的纹理本身
- level：多级渐远纹理的级别。我们将它保留为0。

除了颜色附件之外，我们还可以附加一个深度和模板缓冲纹理到帧缓冲对象中。要附加深度缓冲的话，我们将附件类型设置为`GL_DEPTH_ATTACHMENT`。注意纹理的格式(Format)和内部格式(Internalformat)类型将变为`GL_DEPTH_COMPONENT`，来反映深度缓冲的储存格式。要附加模板缓冲的话，你要将第二个参数设置为`GL_STENCIL_ATTACHMENT`，并将纹理的格式设定为`GL_STENCIL_INDEX`。

也可以将深度缓冲和模板缓冲附加为一个单独的纹理。纹理的每32位数值将包含24位的深度信息和8位的模板信息。要将深度和模板缓冲附加为一个纹理的话，我们使用`GL_DEPTH_STENCIL_ATTACHMENT`类型，并配置纹理的格式，让它包含合并的深度和模板值。将一个深度和模板缓冲附加为一个纹理到帧缓冲的例子可以在下面找到：

```c
glTexImage2D(
  GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, 
  GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
);

glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
```

### 渲染缓冲对象

和纹理图像一样，渲染缓冲对象是一个真正的缓冲，即一系列的字节、整数、像素等。渲染缓冲对象附加的好处是，它会将数据储存为OpenGL原生的渲染格式，它是为离屏渲染到帧缓冲优化过的。

渲染缓冲对象直接将所有的渲染数据储存到它的缓冲中，不会做任何针对纹理格式的转换，让它变为一个更快的可写储存介质。然而，渲染缓冲对象通常都是只写的，所以你不能读取它们（比如使用纹理访问）。当然你仍然还是能够使用`glReadPixels`来读取它，这会从当前绑定的帧缓冲，而不是附件本身，中返回特定区域的像素。

因为它的数据已经是原生的格式了，当写入或者复制它的数据到其它缓冲中时是非常快的。所以，交换缓冲这样的操作在使用渲染缓冲对象时会非常快。

```c
unsigned int rbo;
glGenRenderbuffers(1, &rbo);
glBindRenderbuffer(GL_RENDERBUFFER, rbo);
```

由于渲染缓冲对象通常都是只写的，它们会经常用于深度和模板附件，因为大部分时间我们都不需要从深度和模板缓冲中读取值，只关心深度和模板测试。我们**需要**深度和模板值用于测试，但不需要对它们进行**采样**，所以渲染缓冲对象非常适合它们。当我们不需要从这些缓冲中采样的时候，通常都会选择渲染缓冲对象，因为它会更优化一点。

创建一个深度和模板渲染缓冲对象可以通过调用glRenderbufferStorage函数来完成：

```c
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
```

最后一件事就是附加这个渲染缓冲对象：

```
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
```

> 通常的规则是，如果你不需要从一个缓冲中采样数据，那么对这个缓冲使用渲染缓冲对象会是明智的选择。如果你需要从缓冲中采样颜色或深度值等数据，那么你应该选择纹理附件。性能方面它不会产生非常大的影响的。

### 渲染到纹理

```c
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// 创建一个纹理图像，作为颜色附件附加到帧缓冲上
	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	// 创建渲染缓冲对象，作为深度（和模板）附件到帧缓冲上
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	// 检查帧缓冲是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete!" << std::endl;
	// 解绑帧缓冲，保证我们不会不小心渲染到错误的帧缓冲上
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

要想绘制场景到一个纹理上，我们需要采取以下的步骤：

1. 将新的帧缓冲绑定为激活的帧缓冲，和往常一样渲染场景
2. 绑定默认的帧缓冲
3. 绘制一个横跨整个屏幕的四边形，将帧缓冲的颜色缓冲作为它的纹理。

![](SourceCode/23.Framebuffer/FramebufferTexture2D.png)

线框模式渲染，得到正确结果。

所以这个有什么用处呢？因为我们能够以一个纹理图像的方式访问已渲染场景中的每个像素，我们可以在片段着色器中创建出非常有趣的效果。这些有趣效果统称为**后期处理(Post-processing)效果**。

### 后期处理

**反相**

从屏幕纹理中取颜色值，然后用1.0减去它，对它进行反相：

```c
// screen_frag.glsl
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
	FragColor = vec4(1.0 - texture(screenTexture, TexCoords).rgb, 1.0);
}
```



![](SourceCode/23.Framebuffer/Inversion.png)

**灰度**

```c
// screen_frag.glsl
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
	FragColor = texture(screenTexture, TexCoords);
	float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
	FragColor = vec4(average, average, average, 1.0);
}
```



![](SourceCode/23.Framebuffer/grayscale.png)

**核效果**

在一个纹理图像上做后期处理的另外一个好处是，我们可以从纹理的其它地方采样颜色值。比如说我们可以在当前纹理坐标的周围取一小块区域，对当前纹理值周围的多个纹理值进行采样。我们可以结合它们创建出很有意思的效果。

核(Kernel)（或卷积矩阵(Convolution Matrix)）是一个类矩阵的数值数组，它的中心为当前的像素，它会用它的核值乘以周围的像素值，并将结果相加变成一个值。所以，基本上我们是在对当前像素周围的纹理坐标添加一个小的偏移量，并根据核将结果合并。下面是核的一个例子：

```
2  2  2
2 -15 2
2  2  2
```

这个核取了8个周围像素值，将它们乘以2，而把当前的像素乘以-15。这个核的例子将周围的像素乘上了一个权重，并将当前像素乘以一个比较大的负权重来平衡结果。

> 你在网上找到的大部分核将所有的权重加起来之后都应该会等于1，如果它们加起来不等于1，这就意味着最终的纹理颜色将会比原纹理值更亮或者更暗了。

**锐化效果**

```c
// screen_frag.glsl
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main()
{
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0,     offset),
		vec2(offset,  offset),
		vec2(-offset, 0.0),
		vec2(0.0,     0.0),
		vec2(offset,  0.0),
		vec2(-offset, -offset),
		vec2(0.0,     -offset),
		vec2(offset,  -offset)
		);
	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
		);
	vec3 sampleTex[9];
	for(int i = 0; i < 9; ++i)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; ++i)
	{
		col += sampleTex[i] * kernel[i];
	}

	FragColor = vec4(col, 1.0);
}
```

`const float offset = 1.0 / 300.0;`

![](SourceCode/23.Framebuffer/Sharpen.png)

`const float offset = 1.0 / 30.0;`

![](SourceCode/23.Framebuffer/SharpenToMuch.png)

**模糊效果**

直接替换上面shader代码中的核即可。

```c
	float kernel[9] = float[](
		1.0/16.0, 2.0/16.0, 1.0/16.0,
		2.0/16.0, 4.0/16.0, 2.0/16.0,
		1.0/16.0, 2.0/16.0, 1.0/16.0
		);
```

![](SourceCode/23.Framebuffer/Blur.png)

**边缘检测（基于颜色信息）**

```c
	float kernel[9] = float[](
		1, 1, 1,
		1, -8, 1,
		1, 1, 1
		);
```

![](SourceCode/23.Framebuffer/EdgeDetection.png)

> 注意，核在对屏幕纹理的边缘进行采样的时候，由于还会对中心像素周围的8个像素进行采样，其实会取到纹理之外的像素。由于环绕方式默认是GL_REPEAT，所以在没有设置的情况下取到的是屏幕另一边的像素，而另一边的像素本不应该对中心像素产生影响，这就可能会在屏幕边缘产生很奇怪的条纹。为了消除这一问题，我们可以将屏幕纹理的环绕方式都设置为GL_CLAMP_TO_EDGE。这样子在取到纹理外的像素时，就能够重复边缘的像素来更精确地估计最终的值了。

这里设置屏幕纹理的环绕方式为 `GL_CLAMP_TO_EDGE`

![](SourceCode/23.Framebuffer/EdgeDetectionClampToEdge.png)

**拓展**

基于深度和法线信息进行边缘检测。

首先，需要拿到深度和法线信息。。。

## Day 24 立方体贴图

> 2018.11.26 Cube Map

简单来说，立方体贴图就是一个包含了6个2D纹理的纹理，每个2D纹理都组成了立方体的一个面：一个有纹理的立方体。

![](SourceCode/24.CubeMap/cubemaps_sampling.png)

> 方向向量的大小并不重要，只要提供了方向，OpenGL就会获取方向向量（最终）所击中的纹素，并返回对应的采样纹理值。

如果我们假设将这样的立方体贴图应用到一个立方体上，采样立方体贴图所使用的方向向量将和立方体（插值的）顶点位置非常相像。这样子，只要立方体的中心位于原点，我们就能使用立方体的实际位置向量来对立方体贴图进行采样了。接下来，我们可以将所有顶点的纹理坐标当做是立方体的顶点位置。最终得到的结果就是可以访问立方体贴图上正确面(Face)纹理的一个纹理坐标。

### 创建立方体贴图



```c++
unsigned int textureID;
glGenTextures(1, &textureID);
glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

int width, height, nrChannels;
for (unsigned int i = 0; i < faces.size(); ++i)
{
    unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
                     , 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
        stbi_image_free(data);
    }
}
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
```

不要被`GL_TEXTURE_WRAP_R`吓到，它仅仅是为纹理的**R**坐标设置了环绕方式，它对应的是纹理的第三个维度（和位置的**z**一样）。我们将环绕方式设置为`GL_CLAMP_TO_EDGE`，这是因为正好处于两个面之间的纹理坐标可能不能击中一个面（由于一些硬件限制），所以通过使用`GL_CLAMP_TO_EDGE`，OpenGL将在我们对两个面之间采样的时候，永远返回它们的边界值。

在片段着色器中，我们使用了一个不同类型的采样器，`samplerCube`，我们将使用texture函数使用它进行采样，但这次我们将使用一个`vec3`的方向向量而不是`vec2`。使用立方体贴图的**片段着色器**会像是这样的：

```c
#version 330 core
out vec4 FragColor;

in vec3 textureDir;  // 代表3D纹理坐标的方向向量

uniform samplerCube cubemap; // 立方体贴图的纹理采样器

void main()
{
	FragColor = texture(cubemap, textureDir);
}
```

### 天空盒

绘制天空盒时，我们需要将它变为场景中的第一个渲染的物体，并且禁用深度写入。这样子天空盒就会永远被绘制在其它物体的背后了。

```c
glDepthMask(GL_FALSE);
skyboxShader.use();
// ... 设置观察和投影矩阵
glBindVertexArray(skyboxVAO);
glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
glDrawArrays(GL_TRIANGLES, 0, 36);
glDepthMask(GL_TRUE);
// ... 绘制剩下的场景
```

我们希望移除观察矩阵中的位移部分，让移动不会影响天空盒的位置向量。

```c
view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// 将观察矩阵换为3x3矩阵（移除位移）
skyboxShader.setMat4("view", view);
```

![](SourceCode/24.CubeMap/skybox.png)

### 优化

目前我们是首先渲染天空盒，之后再渲染场景中的其它物体。这样子能够工作，但不是非常高效。如果我们先渲染天空盒，我们就会对屏幕上的每一个像素运行一遍片段着色器，即便只有一小部分的天空盒最终是可见的。可以使用提前深度测试(Early Depth Testing)轻松丢弃掉的片段能够节省我们很多宝贵的带宽。

所以，我们将会最后渲染天空盒，以获得轻微的性能提升。这样子的话，深度缓冲就会填充满所有物体的深度值了，我们只需要在提前深度测试通过的地方渲染天空盒的片段就可以了，很大程度上减少了片段着色器的调用。**我们需要欺骗深度缓冲，让它认为天空盒有着最大的深度值1.0，只要它前面有一个物体，深度测试就会失败。**

在[坐标系统](https://learnopengl-cn.github.io/01%20Getting%20started/08%20Coordinate%20Systems/)小节中我们说过，**透视除法**是在**顶点着色器**运行之后执行的，将gl_Position的`xyz`坐标除以w分量。我们又从[深度测试](https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/01%20Depth%20testing/)小节中知道，相除结果的z分量等于顶点的深度值。使用这些信息，我们可以将输出位置的z分量等于它的w分量，让z分量永远等于1.0，这样子的话，当透视除法执行之后，z分量会变为`w / w = 1.0`。

```c
void main()
{
    vec4 pos = projection * view * vec4(aPos, 1.0);
    TexCoords = aPos;
    gl_Position = pos.xyww;
}
```

### 环境映射

通过使用环境的立方体贴图，我们可以给物体反射和折射的属性。这样使用环境立方体贴图的技术叫做环境映射(Environment Mapping)，其中最流行的两个是反射(Reflection)和折射(Refraction)。

### 反射

反射这个属性表现为物体（或物体的一部分）反射它周围环境，即根据观察者的视角，物体的颜色或多或少等于它的环境。镜子就是一个反射性物体：它会根据观察者的视角反射它周围的环境。

反射的原理并不难。下面这张图展示了我们如何计算反射向量，并如何使用这个向量来从立方体贴图中采样：

![](SourceCode/24.CubeMap/cubemaps_reflection_theory.png)

我们根据观察方向向量和物体的法向量，来计算反射向量。我们可以使用GLSL内建的`reflec`t函数来计算这个反射向量。最终的向量将会作为索引/采样立方体贴图的方向向量，返回环境的颜色值。最终的结果是物体看起来反射了天空盒。

```c
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
```

![](SourceCode/24.CubeMap/reflect.png)

### 折射

折射是通过斯涅尔定律(Snell’s Law)来描述的，使用环境贴图的话看起来像是这样：折射是通过斯涅尔定律(Snell’s Law)来描述的，使用环境贴图的话看起来像是这样：

![](SourceCode/24.CubeMap/cubemaps_refraction_theory.png)

折射可以使用GLSL的内建refract函数来轻松实现，它需要一个法向量、一个观察方向和两个材质之间的折射率(Refractive Index)。

|材质|折射率|
| -- | -- |
|空气|1.00|
|水|1.33|
|冰|1.309|
|玻璃|1.52|
|钻石|2.42|

![](SourceCode/24.CubeMap/refract.png)

### 动态环境贴图[TODO]



现在我们使用的都是静态图像的组合来作为天空盒，看起来很不错，但它没有在场景中包括可移动的物体。我们一直都没有注意到这一点，因为我们只使用了一个物体。如果我们有一个镜子一样的物体，周围还有多个物体，镜子中可见的只有天空盒，看起来就像它是场景中唯一一个物体一样。

通过使用帧缓冲，我们能够为物体的6个不同角度创建出场景的纹理，并在每个渲染迭代中将它们储存到一个立方体贴图中。之后我们就可以使用这个（动态生成的）立方体贴图来创建出更真实的，包含其它物体的，反射和折射表面了。这就叫做动态环境映射(Dynamic Environment Mapping)，因为我们动态创建了物体周围的立方体贴图，并将其用作环境贴图。

虽然它看起来很棒，但它有一个很大的缺点：我们需要为使用环境贴图的物体渲染场景6次，这是对程序是非常大的性能开销。现代的程序通常会尽可能使用天空盒，并在可能的时候使用预编译的立方体贴图，只要它们能产生一点动态环境贴图的效果。虽然动态环境贴图是一个很棒的技术，但是要想在不降低性能的情况下让它工作还是需要非常多的技巧的。

### 练习

反射贴图。。。todo

