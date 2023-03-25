#pragma once

#include <GLFW/glfw3.h>

class CallBack
{
private:
	CallBack();

public:
	static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CharCallBack(GLFWwindow* window, unsigned int codepoint);
	static void FrameBufferResizeCallBack(GLFWwindow* window, int width, int height);
	static void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
};