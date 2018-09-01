#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <learnopengl\shader_s.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	///////////////////////////////////////////////
	// glfw: 创建窗口
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	///////////////////////////////////////////////
	// glfw: 注册回调函数
	// 注册窗口回调函数，调整视口大小
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	///////////////////////////////////////////////
	// glad: 初始化GLAD，管理OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	///////////////////////////////////////////////
	// build and compile our shader program
	// vertex shader
	Shader ourShader("src/vert.glsl", "src/frag.glsl");

	///////////////////////////////////////////////
	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// 位置				// 颜色
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	0.f,	// left
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	// right
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f	// top
	};

	unsigned int VAO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attribut(s)
	glBindVertexArray(VAO);

	// 复制顶点数组到缓冲中供 OpenGL 使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VOBs) when it's not directly necessary
	//glBindVertexArray(0);

	// bind the VAO (it was already bound, but just to demonstrate) : seeing as we only have a single VAO and we can
	// just bind it beforehand before rendering the respective triangle; this is another approach
	glBindVertexArray(VAO);

	// calculate fps
	float lastTime = glfwGetTime();
	int cntFrames = 0;

	///////////////////////////////////////////////
	// 渲染循环（Render Loop）
	while (!glfwWindowShouldClose(window))
	{
		// calculate fps
		float currentTime = glfwGetTime();
		++cntFrames;
		if (currentTime - lastTime >= 1.0f)
		{
			std::cout << cntFrames << " fps" << std::endl;
			cntFrames = 0;
			lastTime = currentTime;
		}

		// 输入
		processInput(window);

		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// 状态设置函数
		glClear(GL_COLOR_BUFFER_BIT);			// 状态使用函数

		// 激活着色器程序
		ourShader.use();

		// 更新 uniform
		//float timeValue = glfwGetTime();
		//float lateralOffset = sin(timeValue);
		ourShader.setFloat ("lateralOffset", 0.5f);

		// 绘制三角形
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// glfw: 交换颜色缓冲，检查输入事件（keys pressed/released, mouse moved etc）
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	///////////////////////////////////////////////
	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	///////////////////////////////////////////////
	// 终止，释放所有之前分配的GLFW资源
	glfwTerminate();
	return 0;
}

// glfw: 回调函数，当窗口改变大小时，GLFW会调用这个函数并填充相应的参数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// 处理所有输入按键: 向GLFW查询是否有相关的按键被按下/释放，并作出相应的反应
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}