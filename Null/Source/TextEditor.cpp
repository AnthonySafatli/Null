#include "Headers/TextEditor.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "GLFW/glfw3.h"

#include "Headers/Program.h"
#include "Headers/Character.h"
#include "Headers/Uniforms.h"

std::vector<std::string> Split(const std::string str, const char separator);

extern Program program;

TextEditor::TextEditor() : fileDirectory(""), fileName("")
{
	if (program.openGL.init)
		SetLeftMargin(4);
	else
		leftMargin = 4;

	program.textX = 0;
	program.textY = 0;

	rows.push_back(std::string());

	// add ' '
	TexCoords spaceTexCoords = GetCoords(' ');
	program.marginVertices.push_back(Vertex(0.0, 0.0, spaceTexCoords.u               , spaceTexCoords.v               , 1, -3, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 0.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v               , 1, -3, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 1.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v + (1.0 / 10.0), 1, -3, MARGIN));
	program.marginVertices.push_back(Vertex(0.0, 1.0, spaceTexCoords.u               , spaceTexCoords.v + (1.0 / 10.0), 1, -3, MARGIN));
				
	// add ' '
	program.marginVertices.push_back(Vertex(0.0, 0.0, spaceTexCoords.u               , spaceTexCoords.v               , 1, -2, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 0.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v               , 1, -2, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 1.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v + (1.0 / 10.0), 1, -2, MARGIN));
	program.marginVertices.push_back(Vertex(0.0, 1.0, spaceTexCoords.u               , spaceTexCoords.v + (1.0 / 10.0), 1, -2, MARGIN));
				
	// add '1'
	TexCoords oneTexCoords = GetCoords('1');
	program.marginVertices.push_back(Vertex(0.0, 0.0,   oneTexCoords.u               ,   oneTexCoords.v               , 1, -1, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 0.0,   oneTexCoords.u + (1.0 / 10.0),   oneTexCoords.v               , 1, -1, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 1.0,   oneTexCoords.u + (1.0 / 10.0),   oneTexCoords.v + (1.0 / 10.0), 1, -1, MARGIN));
	program.marginVertices.push_back(Vertex(0.0, 1.0,   oneTexCoords.u               ,   oneTexCoords.v + (1.0 / 10.0), 1, -1, MARGIN));

	program.ShowCursor();
	program.showCursor = true;

	program.RenderStatus("New Editor Loaded Successfully");
}

TextEditor::TextEditor(const std::string text, const std::string directory, const std::string fileName) : fileName(fileName), fileDirectory(directory)
{
	if (program.openGL.init)
		SetLeftMargin(4);
	else
		leftMargin = 4;

	program.textX = 0;
	program.textY = 0;

	rows.push_back(std::string());

	// add ' '
	TexCoords spaceTexCoords = GetCoords(' ');
	program.marginVertices.push_back(Vertex(0.0, 0.0, spaceTexCoords.u               , spaceTexCoords.v               , 1, -3, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 0.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v               , 1, -3, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 1.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v + (1.0 / 10.0), 1, -3, MARGIN));
	program.marginVertices.push_back(Vertex(0.0, 1.0, spaceTexCoords.u               , spaceTexCoords.v + (1.0 / 10.0), 1, -3, MARGIN));

	// add ' '
	program.marginVertices.push_back(Vertex(0.0, 0.0, spaceTexCoords.u               , spaceTexCoords.v               , 1, -2, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 0.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v               , 1, -2, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 1.0, spaceTexCoords.u + (1.0 / 10.0), spaceTexCoords.v + (1.0 / 10.0), 1, -2, MARGIN));
	program.marginVertices.push_back(Vertex(0.0, 1.0, spaceTexCoords.u               , spaceTexCoords.v + (1.0 / 10.0), 1, -2, MARGIN));

	// add '1'
	TexCoords oneTexCoords = GetCoords('1');
	program.marginVertices.push_back(Vertex(0.0, 0.0, oneTexCoords.u               , oneTexCoords.v               , 1, -1, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 0.0, oneTexCoords.u + (1.0 / 10.0), oneTexCoords.v               , 1, -1, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 1.0, oneTexCoords.u + (1.0 / 10.0), oneTexCoords.v + (1.0 / 10.0), 1, -1, MARGIN));
	program.marginVertices.push_back(Vertex(0.0, 1.0, oneTexCoords.u               , oneTexCoords.v + (1.0 / 10.0), 1, -1, MARGIN));

	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] == '\n')
		{
			Return();
			continue;
		}

		if (text[i] == '\t')
		{
			AddTab();
			continue;
		}

		AddCharacter(text[i]);
	}

	program.textX = 0;
	program.textY = 0;
	program.RenderStatus(fileName + " Loaded Successfully");
}

void TextEditor::ProcessKey(int key, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	switch (key)
	{
	case KEYCODE_HOME:
		MoveHome();
		UpdateRowColVisual();
		break;
	case KEYCODE_END:
		MoveEnd();
		UpdateRowColVisual();
		break;
	case KEYCODE_RIGHT:
		MoveRight();
		UpdateRowColVisual();
		break;
	case KEYCODE_LEFT:
		MoveLeft();
		UpdateRowColVisual();
		break;
	case KEYCODE_UP:
		MoveUp();
		UpdateCol();
		UpdateRowColVisual();
		break;
	case KEYCODE_DOWN:
		MoveDown();
		UpdateCol();
		UpdateRowColVisual();
		break;

	case KEYCODE_ENTER:
		Return();
		UpdateRowColVisual();
		break;
	case KEYCODE_DEL:
		RemoveCharacterFromLeft();
		UpdateRowColVisual();
		break;
	case KEYCODE_DELETE:
		RemoveCharacterFromRight();
		UpdateRowColVisual();
		break;
	case KEYCODE_TAB:
		AddTab();
		UpdateRowColVisual();
		break;
	}
}

void TextEditor::ProcessChar(unsigned int codepoint)
{
	if (!(codepoint > 31 && codepoint < 128))
		return;
	
	AddCharacter((char)codepoint);
	UpdateRowColVisual();
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
			AddCharacterToMargin(rowNumberString[index], -leftMargin + i + 1, MARGIN);

			continue;
		}

		AddCharacterToMargin(' ', -leftMargin + i + 1, MARGIN);
	}
}

void TextEditor::RemoveLeftMargin()
{
	program.marginVertices.resize(program.marginVertices.size() - (4 * (leftMargin - 1)));
	program.indices.resize(program.indices.size() - (6 * (leftMargin - 1)));
}

void TextEditor::IncreaseLeftMargin()
{
	TexCoords texCoords = GetCoords(' ');

	std::vector<Vertex> spaceVertices;

	spaceVertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , 0, -leftMargin, MARGIN));
	spaceVertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , 0, -leftMargin, MARGIN));
	spaceVertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), 0, -leftMargin, MARGIN));
	spaceVertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), 0, -leftMargin, MARGIN));

	for (int i = 0; i < rows.size(); i++)
	{
		int index = i * leftMargin * 4;

		for (Vertex spaceVertex : spaceVertices) spaceVertex.row = i;

		program.marginVertices.insert(program.marginVertices.begin() + index, spaceVertices.begin(), spaceVertices.end());
	}

	SetLeftMargin(leftMargin + 1);
}

/// <summary>
/// If you move up or down to a shorter row, the cursor will move to the end of the shorter row rather than hang out in empty space 
/// </summary>
void TextEditor::UpdateCol()
{
	if (program.textX > rows[program.textY].size())
		MoveEnd();
}

/// <summary>
/// Make sure the cursor is visible by updating row / column Index
/// </summary>
void TextEditor::UpdateRowColVisual()
{
	int rows = ((1.0 / ((float)program.textSize * 0.001)) * ((float)program.height / (float)program.idealHeight)) - 4;
	int cols = ((1.0 / ((float)program.textSize * 0.001)) * ((float)program.width / (float)program.idealWidth)) - 4;

	if ((int)program.rowIndex + rows - 1 < (int)program.textY) // if cursor if under the viewport
	{
		program.rowIndex += program.textY - ((int)program.rowIndex + rows) + 1;
		UpdateUniform1i(program.openGL.u_rowIndex.location, (int)program.rowIndex);
	}
	else if ((int)program.rowIndex > (int)program.textY) // if cursor is above the viewport
	{
		program.rowIndex = (int)program.textY;
		UpdateUniform1i(program.openGL.u_rowIndex.location, (int)program.rowIndex);
	}

	// TODO: Debug left (after vertex.glsl remake)
	if ((int)program.columnIndex + cols - 1 < (int)program.textX) // if cursor if right of the viewport
	{
		program.columnIndex += program.textX - ((int)program.columnIndex + cols) + 1;
		UpdateUniform1i(program.openGL.u_columnIndex.location, (int)program.columnIndex);
	}
	else if ((int)program.columnIndex > (int)program.textX) // if cursor is left of the viewport
	{
		program.columnIndex = (int)program.textX;
		UpdateUniform1i(program.openGL.u_columnIndex.location, (int)program.columnIndex);
	}
}

/// <summary>
/// Make sure the cursor is visible by updating row / column Index
/// </summary>
void TextEditor::UpdateColVisual()
{
	int cols = ((1.0 / ((float)program.textSize * 0.001)) * ((float)program.width / (float)program.idealWidth)) - 4;

	if ((int)program.columnIndex + cols - 1 < (int)program.textX) // if cursor if under the viewport
	{
		program.columnIndex += program.textX - ((int)program.columnIndex + cols) + 1;
		UpdateUniform1i(program.openGL.u_columnIndex.location, (int)program.columnIndex);
		return;
	}

	if ((int)program.columnIndex > (int)program.textX) // if cursor is above the viewport
	{
		program.columnIndex = (int)program.textX;
		UpdateUniform1i(program.openGL.u_columnIndex.location, (int)program.columnIndex);
	}
}

std::string TextEditor::GetText()
{
	std::stringstream ss;

	for (int i = 0; i < rows.size(); i++)
	{
		ss << rows[i];

		if (i + 1 < rows.size())
			ss << '\n';
	}

	return ss.str();
}

void TextEditor::SetPath(std::string path)
{
	std::vector<std::string> pathVec = Split(path, '\\');
	std::stringstream ss;
	for (int i = 0; i < pathVec.size() - 1; i++) ss << pathVec[i] + '\\';
	fileDirectory = ss.str();
	
	std::string file = pathVec[pathVec.size() - 1];
	fileName = file;
}

