#include "Headers/CallBack.h"
#include "Headers/NullEditor.h"
#include "Headers/Program.h"

/* ====== CallBack ====== */

extern NullEditor appContents;
extern Program program;

void CallBack::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	appContents.ProcessKey(key, action, mods);
	program.ProcessKey(key, action, mods);
}

void CallBack::CharCallBack(GLFWwindow* window, unsigned int codepoint)
{
	appContents.ProcessChar(codepoint);
	program.ProcessChar(codepoint);
}

void CallBack::FrameBufferResizeCallBack(GLFWwindow* window, int width, int height)
{
	appContents.OnResize(width, height);
	program.OnResize(width, height);
}
