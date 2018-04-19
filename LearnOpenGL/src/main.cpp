#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
	// ��Ⱦѭ����Render Loop��
	while (!glfwWindowShouldClose(window))
	{
		// ����
		processInput(window);

		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// ״̬���ú���
		glClear(GL_COLOR_BUFFER_BIT);			// ״̬ʹ�ú���

		// glfw: ������ɫ���壬��������¼���keys pressed/released, mouse moved etc��
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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
}