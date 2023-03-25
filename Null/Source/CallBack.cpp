#include "Headers/CallBack.h"
#include "Headers/Program.h"

/* ====== CallBack ====== */

extern Program program;

void CallBack::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	program.ProcessKey(key, action, mods);
}

void CallBack::CharCallBack(GLFWwindow* window, unsigned int codepoint)
{
	program.ProcessChar(codepoint);
}

void CallBack::FrameBufferResizeCallBack(GLFWwindow* window, int width, int height)
{
	program.OnResize(width, height);
}

void CallBack::ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	program.OnScroll(xoffset, yoffset);
}