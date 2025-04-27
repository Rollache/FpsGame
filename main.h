#pragma once
#include<windows.h>
#include <string>
#include "base.h"
#include "world.h"
#include "SceneManager.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"GameClearScene.h"
//#include<vld.h>

#define TARGET_FRAME  0.016667
#define FRAME_ALPHA  0.25
#define FPS 60

GLFWwindow* window = nullptr;
glm::vec2 windowSize;

void OpenWindow()
{
	const char* title = "Arrow Sprinter";

	if (!glfwInit())
	{
		cout << "Could not initialize GLFW" << endl;
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_REFRESH_RATE, FPS);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (!monitor)
	{
		std::cerr << "Failed to get primary monitor" << std::endl;
		glfwTerminate();
		return;
	}

	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	//FULL SCREEN
	window = glfwCreateWindow(videoMode->width, videoMode->height, title, monitor, NULL);

	//WINDOW
	//window = glfwCreateWindow(videoMode->width, videoMode->height, title, NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//application icon
	GLFWimage images[1];
	images[0].pixels = stbi_load("res/texture/icon/icon.png", &images[0].width, &images[0].height, 0, 4);
	if (images[0].pixels)
	{
		glfwSetWindowIcon(window, 1, images);
		stbi_image_free(images[0].pixels);
	}
	else
	{
		std::cerr << "Failed to load icon.png" << std::endl;
	}

	//マウスカーソルを無効化
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glGetError();

	windowSize = glm::vec2(videoMode->width, videoMode->height);
}

void PrepareOpenGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

