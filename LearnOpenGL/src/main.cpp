#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 vertex_position;\n"
"layout (location = 1) in vec3 vertex_color;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"color = vertex_color;\n"
"gl_Position = vec4(vertex_position, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 color;\n"
"out vec4 frag_color;\n"
"void main()\n"
"{\n"
"frag_color = vec4(color, 1.0);\n"
"}\n\0";

// ui interactions
enum DrawMode{Fill, Wire};

DrawMode drawMode = DrawMode::Fill;

int main()
{
	///////////////////////////////////////////////
	// glfw: ��������
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
	// glfw: ע��ص�����
	// ע�ᴰ�ڻص������������ӿڴ�С
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	///////////////////////////////////////////////
	// glad: ��ʼ��GLAD������OpenGL����ָ��
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	///////////////////////////////////////////////
	// build and compile our shader program
	// vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// �ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	///////////////////////////////////////////////
	// set up vertex data (and buffer(s)) and configure vertex attributes
	float points[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	// left (pos + color)
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	// right(pos + color)
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f	// top  (pos + color)
	};

	unsigned int vao = 0;
	unsigned int points_vbo = 0;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &points_vbo);

	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	///////////////////////////////////////////////
	// ��Ⱦѭ����Render Loop��
	while (!glfwWindowShouldClose(window))
	{
		// ����
		processInput(window);

		switch (drawMode)
		{
		case Fill:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case Wire:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		default:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}

		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// ״̬���ú���
		glClear(GL_COLOR_BUFFER_BIT);			// ״̬ʹ�ú���

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);	// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // no need to unbind it every time

		// glfw: ������ɫ���壬��������¼���keys pressed/released, mouse moved etc��
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	///////////////////////////////////////////////
	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &points_vbo);

	///////////////////////////////////////////////
	// ��ֹ���ͷ�����֮ǰ�����GLFW��Դ
	glfwTerminate();
	return 0;
}

// glfw: �ص������������ڸı��Сʱ��GLFW�������������������Ӧ�Ĳ���
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// �����������밴��: ��GLFW��ѯ�Ƿ�����صİ���������/�ͷţ���������Ӧ�ķ�Ӧ
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		drawMode = DrawMode::Wire;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		drawMode = DrawMode::Fill;
}