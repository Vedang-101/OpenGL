#pragma once

#include<stdio.h>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

class Window {
private:
	GLFWwindow *mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	//Mouse Positions
	GLfloat lastX, lastY;
	GLfloat xChange, yChange;
	bool mouseFirstMoved;

	void CreateCallbacks();
	static void HandleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow *window, double xPos, double yPos);

public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetBufferHeight() { return bufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* GetKeys() { return keys; }
	
	GLfloat GetXMouseChange();
	GLfloat GetYMouseChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();
};

