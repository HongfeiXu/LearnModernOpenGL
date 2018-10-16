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

