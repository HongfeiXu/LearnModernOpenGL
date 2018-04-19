# LearnOpenGL

> 想继续深入学习计算机图形学。之前有些OpenGL编程的经验，不想就此给扔了（毕竟重头开始学习DirectX也需要花些时间，倒不如先把原理弄清，在战略转移）因此，再次重拾这个系列教程，发现教程又更新了不少内容，因此更加适合温故而知新了。所以，开始吧。
>
> Date: 2018.4.19
>
> Website: https://learnopengl-cn.github.io/

## Day 1

> Date: 2018.4.19

Getting started::OpenGL

Getting started::Creating a window

Getting started::Hello Window

### 配置库

| 库名                                      | 作用                                           |
| ----------------------------------------- | ---------------------------------------------- |
| [GLFW](http://www.glfw.org/download.html) | 创建OpenGL上下文，定义窗口函数以及处理用户输入 |
| [GLAD](http://glad.dav1d.de/)             | 在运行时获取OpenGL函数地址                     |

头文件以及库文件在[这里](Librarys)。

我们要做的是，将头文件目录（Library\Includes）添加到工程的包含目录中，将库文件目录（Library\Libs）添加到工程的库目录中，在链接器中添加附加依赖项**opengl32.lib**，**glfw3.lib**，并且将glad.c添加到工程中。如下图。

![ProjectSetting](SourceCode\HelloWindow\ProjectSetting.jpg)



### 2. 输出了一个窗口！并且可以通过Esc按键关闭窗口！

![HelloWindow](SourceCode\HelloWindow\screenshot.png)

代码在[这里](SourceCode/HelloWindow/main.cpp)

