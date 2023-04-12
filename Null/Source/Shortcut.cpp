#include "Headers/Shortcut.h"

#include "GLFW/glfw3.h"

#include "Headers/Program.h"
#include "Headers/Command.h"

extern Program program;

bool Shortcut::ctrlPressed()
{
	int leftCtrl = glfwGetKey(program.window, GLFW_KEY_LEFT_CONTROL);
	int rightCtrl = glfwGetKey(program.window, GLFW_KEY_RIGHT_CONTROL);

	return leftCtrl != GLFW_RELEASE || rightCtrl != GLFW_RELEASE;
}

bool Shortcut::shftPressed()
{
	int leftShft = glfwGetKey(program.window, GLFW_KEY_LEFT_SHIFT);
	int rightShft = glfwGetKey(program.window, GLFW_KEY_RIGHT_SHIFT);

	return leftShft != GLFW_RELEASE || rightShft != GLFW_RELEASE;
}

void Shortcut::Save()
{
	Command::Execute("save");
}

void Shortcut::Open()
{
	Command::Execute("open");
}

void Shortcut::UniversalShortcuts(const int key, const int action)
{
	if (action != GLFW_PRESS)
		return;

	if (!ctrlPressed())
		return;

	if (key == GLFW_KEY_S)
		Save();
	else if (key == GLFW_KEY_O)
		Open();
}

void Shortcut::CommandShortcuts(const int key, const int action)
{
	if (action != GLFW_PRESS)
		return;

	if (!ctrlPressed())
		return;
}

void Shortcut::TextEditorShortcuts(const int key, const int action)
{
	if (action != GLFW_PRESS)
		return;

	if (!ctrlPressed())
		return;
}
