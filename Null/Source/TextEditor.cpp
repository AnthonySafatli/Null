#include "Headers/TextEditor.h"

#include <iostream>

#include "GLFW/glfw3.h"

#include "Headers/Program.h"

extern Program program;

TextEditor::TextEditor() 
{
	leftMargin = 4;
	// SetLeftMargin(4);
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

	case KEYCODE_ENTER:
		Return();
		break;
	case KEYCODE_DEL:
		RemoveCharacterFromLeft();
		break;
	case KEYCODE_DELETE:
		RemoveCharacterFromRight();
		break;
	case KEYCODE_TAB:
		AddTab();
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

void TextEditor::AddLeftMargin()
{
	std::string rowNumberString = std::to_string(rows.size());

	for (int i = 0; i < leftMargin - 1; i++)
	{
		if (leftMargin - i - 1 <= rowNumberString.size())
		{
			AddCharacterToMargin(rowNumberString.at(leftMargin - i - 2), -leftMargin + i);

			continue;
		}

		AddCharacterToMargin(' ', -leftMargin + i);
	}

	AddCharacterToMargin(' ', -1);
}

void TextEditor::IncreaseLeftMargin()
{

}

