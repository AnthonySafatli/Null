#include "Headers/TextEditor.h"

#include <iostream>

#include "GLFW/glfw3.h"

#include "Headers/Program.h"
#include "Headers/Character.h"

extern Program program;

TextEditor::TextEditor() 
{
	leftMargin = 4;
	// SetLeftMargin(4);
	rows.push_back(std::string());

	TexCoords spaceTexCoords = GetCoords(' ');
	TexCoords oneTexCoords = GetCoords('1');

	program.marginVertices.push_back(Vertex(0.0, 0.0, spaceTexCoords.u               , spaceTexCoords.v               , 1, -3, 0.0));
	program.marginVertices.push_back(Vertex(1.0, 0.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v               , 1, -3, 0.0));
	program.marginVertices.push_back(Vertex(1.0, 1.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v + (1.0 / 10.0), 1, -3, 0.0));
	program.marginVertices.push_back(Vertex(0.0, 1.0, spaceTexCoords.u               , spaceTexCoords.v + (1.0 / 10.0), 1, -3, 0.0));
																														
	program.marginVertices.push_back(Vertex(0.0, 0.0, spaceTexCoords.u               , spaceTexCoords.v               , 1, -2, 0.0));
	program.marginVertices.push_back(Vertex(1.0, 0.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v               , 1, -2, 0.0));
	program.marginVertices.push_back(Vertex(1.0, 1.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v + (1.0 / 10.0), 1, -2, 0.0));
	program.marginVertices.push_back(Vertex(0.0, 1.0, spaceTexCoords.u               , spaceTexCoords.v + (1.0 / 10.0), 1, -2, 0.0));
																												
	program.marginVertices.push_back(Vertex(0.0, 0.0,   oneTexCoords.u               ,   oneTexCoords.v               , 1, -1, 0.0));
	program.marginVertices.push_back(Vertex(1.0, 0.0,   oneTexCoords.u + (1.0 / 10.0),   oneTexCoords.v               , 1, -1, 0.0));
	program.marginVertices.push_back(Vertex(1.0, 1.0,   oneTexCoords.u + (1.0 / 10.0),   oneTexCoords.v + (1.0 / 10.0), 1, -1, 0.0));
	program.marginVertices.push_back(Vertex(0.0, 1.0,   oneTexCoords.u               ,   oneTexCoords.v + (1.0 / 10.0), 1, -1, 0.0));
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

	if (rowNumberString.size() > leftMargin - 1)
	     IncreaseLeftMargin();

	for (int i = 0; i < leftMargin - 1; i++)
	{
		if (leftMargin - i - 1 <= rowNumberString.size())
		{
			int index = i - (leftMargin - rowNumberString.size() - 1);
			AddCharacterToMargin(rowNumberString[index], -leftMargin + i + 1);

			continue;
		}

		AddCharacterToMargin(' ', -leftMargin + i);
	}
}

void TextEditor::IncreaseLeftMargin()
{
	TexCoords texCoords = GetCoords(' ');

	std::vector<Vertex> spaceVertices;

	spaceVertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , 0, -leftMargin, 0.0));
	spaceVertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , 0, -leftMargin, 0.0));
	spaceVertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), 0, -leftMargin, 0.0));
	spaceVertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), 0, -leftMargin, 0.0));

	for (int i = 0; i < rows.size(); i++)
	{
		int index = i * leftMargin * 4;

		for (Vertex spaceVertex : spaceVertices) spaceVertex.row = i;

		program.marginVertices.insert(program.marginVertices.begin() + index, spaceVertices.begin(), spaceVertices.end());
	}

	SetLeftMargin(leftMargin + 1);
}
