#include "Headers/TextEditor.h"

#include <iostream>

#include "GLFW/glfw3.h"

#include "Headers/Program.h"

TextEditor::TextEditor()
{
	rows.push_back(std::string());
}

void TextEditor::ProcessKey(int key, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	switch (key)
	{
	case KEYCODE_HOME:
		MoveHome();
		break;
	case KEYCODE_END:
		MoveEnd();
		break;
	case KEYCODE_RIGHT:
		MoveRight();
		break;
	case KEYCODE_LEFT:
		MoveLeft();
		break;
	case KEYCODE_UP:
		MoveUp();
		break;
	case KEYCODE_DOWN:
		MoveDown();
		break;
	}
}

void TextEditor::ProcessChar(unsigned int codepoint)
{
	if (!(codepoint > 31 && codepoint < 128))
		return;

	AddCharacter((char)codepoint);
}

void TextEditor::OnResize(int width, int height)
{
}

