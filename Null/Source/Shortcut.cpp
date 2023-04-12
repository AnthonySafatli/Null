#include "Headers/Shortcut.h"

#include "GLFW/glfw3.h"

#include <iostream>

#include "Headers/Program.h"
#include "Headers/Command.h"

extern Program program;

void Shortcut::SaveAs()
{
	Command::Execute("save as");
}

void Shortcut::Save()
{
	Command::Execute("save");
}

void Shortcut::Open()
{
	Command::Execute("open");
}

void Shortcut::OpenNew()
{
	Command::Execute("open new");
}

void Shortcut::Copy()
{
#if _DEBUG
	std::cout << "Copy Not Implemented yet" << std::endl;
#endif
}

void Shortcut::Paste()
{
#if _DEBUG
	std::cout << "Paste Not Implemented yet" << std::endl;
#endif
}

void Shortcut::Cut()
{
#if _DEBUG
	std::cout << "Cut Not Implemented yet" << std::endl;
#endif
}

void Shortcut::Undo()
{
#if _DEBUG
	std::cout << "Undo Not Implemented yet" << std::endl;
#endif
}

void Shortcut::Redo()
{
#if _DEBUG
	std::cout << "Redo Not Implemented yet" << std::endl;
#endif
}

void Shortcut::Duplicate()
{
#if _DEBUG
	std::cout << "Duplicate Not Implemented yet" << std::endl;
#endif
}

void Shortcut::Refresh()
{
	Command::Execute("refresh");
}

void Shortcut::ScrollUp()
{
#if _DEBUG
	std::cout << "Scroll Not Implemented yet" << std::endl;
#endif
}

void Shortcut::ScrollDown()
{
#if _DEBUG
	std::cout << "Scroll Not Implemented yet" << std::endl;
#endif
}

void Shortcut::SizeUp()
{
	Command::Execute("size +");
}

void Shortcut::SizeDown()
{
	Command::Execute("size -");
}

void Shortcut::UniversalShortcuts(int key, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	bool ctrlPressed = (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL;
	bool shftPressed = (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT;

	if (!ctrlPressed)
		return;

	if (shftPressed && key == GLFW_KEY_S)
		SaveAs();
	else if (key == GLFW_KEY_S)
		Save();
	else if (key == GLFW_KEY_O)
		Open();
	else if (key == GLFW_KEY_N)
		OpenNew();
	else if (key == GLFW_KEY_C)
		Copy();
	else if (key == GLFW_KEY_V)
		Paste();
	else if (key == GLFW_KEY_X)
		Cut();
	else if (key == GLFW_KEY_Z)
		Undo();
	else if (key == GLFW_KEY_Y)
		Redo();
	else if (key == GLFW_KEY_D)
		Duplicate();
	else if (key == GLFW_KEY_R)
		Refresh();
	else if (key == GLFW_KEY_UP)
		ScrollUp();
	else if (key == GLFW_KEY_DOWN)
		ScrollDown();
	else if (key == GLFW_KEY_EQUAL)
		SizeUp();
	else if (key == GLFW_KEY_MINUS)
		SizeDown();
}

void Shortcut::CommandShortcuts(int key, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;
}

void Shortcut::TextEditorShortcuts(int key, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;
}
