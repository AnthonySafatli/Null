#include "Headers/CallBack.h"
#include "Headers/NullEditor.h"

/* ====== CallBack ====== */

extern NullEditor appContents;

void CallBack::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	appContents.ProcessKey(key, action, mods);
}

void CallBack::CharCallBack(GLFWwindow* window, unsigned int codepoint)
{
	appContents.ProcessChar(codepoint);
}

void CallBack::FrameBuffeResizeCallBack(GLFWwindow* window, int width, int height)
{
	appContents.OnResize(width, height);
}
