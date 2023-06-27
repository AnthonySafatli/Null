#include "Headers/TextEditor.h"

#include "GLFW/glfw3.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <ctype.h>

#include "Headers/TextArea.h"
#include "Headers/Program.h"
#include "Headers/Character.h"
#include "Headers/Uniforms.h"
#include "Headers/Shortcut.h"
#include "Headers/UndoObject.h"

std::vector<std::string> Split(const std::string str, const char separator);

extern Program program;

TextEditor::TextEditor() : fileDirectory(""), fileName(""), isNote(false)
{
	type = EDITOR;

	ConstructorStart(4, true);
	program.RenderStatus("New Editor Loaded Successfully");
}

TextEditor::TextEditor(const std::string text, const std::string directory, const std::string fileName, bool isNote) : fileName(fileName), fileDirectory(directory), isNote(isNote)
{
	type = EDITOR;

	ConstructorStart(4, true);

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

	program.textY = rows.size() - 1;
	MoveEnd();

	if (program.openGL.init)
		UpdateUniform1i(program.openGL.u_cursorRow.location, rows.size());

	program.RenderStatus(fileName + " Loaded Successfully");
}

void TextEditor::ProcessKey(int key, int action, int mods) 
{
	if (action == GLFW_RELEASE)
		return;

	Shortcut::TextEditorShortcuts(key, action, mods);

	if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
	{
		switch (key)
		{
		case GLFW_KEY_LEFT:
			MoveLeftCtrl();
			break;
		case GLFW_KEY_RIGHT:
			MoveRightCtrl();
			break;
		}
	
		return;
	}

	bool numLock = (mods & GLFW_MOD_NUM_LOCK) == GLFW_MOD_NUM_LOCK;

	switch (key)
	{
	case GLFW_KEY_HOME:
		MoveHome();
		UpdateRowColVisual();
		break;
	case GLFW_KEY_KP_7: 
		// TODO: [END] Numpad not working
		if (glfwGetKey(program.window, GLFW_KEY_NUM_LOCK) != GLFW_PRESS)
			break;
		MoveHome();
		UpdateRowColVisual();
		break;
	case GLFW_KEY_END:
		MoveEnd();
		UpdateRowColVisual();
		break;
	case GLFW_KEY_KP_1:
		// TODO: [END] Numpad not working
		if (glfwGetKey(program.window, GLFW_KEY_NUM_LOCK) != GLFW_PRESS)
			break;
		MoveEnd();
		UpdateRowColVisual();
		break;
	case GLFW_KEY_RIGHT:
		MoveRight();
		UpdateRowColVisual();
		break;
	case GLFW_KEY_LEFT:
		MoveLeft();
		UpdateRowColVisual();
		break;
	case GLFW_KEY_UP:
		MoveUp();
		UpdateCol();
		UpdateRowColVisual();
		break;
	case GLFW_KEY_DOWN:
		MoveDown();
		UpdateCol();
		UpdateRowColVisual();
		break;

	case GLFW_KEY_ENTER:
		undoStack.push(UndoObject(rows, program.textX, program.textY));
		while (!redoStack.empty()) redoStack.pop();
		Return();
		UpdateRowColVisual();
		break;
	case GLFW_KEY_BACKSPACE:
		undoStack.push(UndoObject(rows, program.textX, program.textY));
		while (!redoStack.empty()) redoStack.pop();
		RemoveCharacterFromLeft();
		UpdateRowColVisual();
		break;
	case GLFW_KEY_DELETE:
		undoStack.push(UndoObject(rows, program.textX, program.textY));
		while (!redoStack.empty()) redoStack.pop();
		RemoveCharacterFromRight();
		UpdateRowColVisual();
		break;
	case GLFW_KEY_TAB:
		undoStack.push(UndoObject(rows, program.textX, program.textY));
		while (!redoStack.empty()) redoStack.pop();
		AddTab();
		UpdateRowColVisual();
		break;
	}

	UpdateUniform1i(program.openGL.u_cursorRow.location, program.textY + 1);
}

void TextEditor::ProcessChar(unsigned int codepoint)
{
	if (!(codepoint > 31 && codepoint < 128))
		return;
	
	undoStack.push(UndoObject(rows, program.textX, program.textY));
	while (!redoStack.empty()) redoStack.pop();
	AddCharacter((char)codepoint);
	UpdateRowColVisual();
}

void TextEditor::AddLeftMargin()
{
	int rowNumber = std::floor(program.marginVertices.size() / 4 / (leftMargin - 1)) + 1;

	std::string rowNumberString = std::to_string(rowNumber);

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

void TextEditor::UpdateCol()
{
	if (program.textX > rows[program.textY].size())
		MoveEnd();
}

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

void TextEditor::MoveLeftCtrl()
{
	bool isAtText;

	if (program.textX == 0)
	{
		if (program.textY == 0)
			return;

		program.textY--;
		MoveEnd();
	}

	while (true)
	{
		if (program.textX == 0)
			return;

		if (isWhiteSpace(rows[program.textY][(program.textX--) - 1]))
			continue;

		isAtText = isText(rows[program.textY][(++program.textX) - 1]);
		break;
	}

	while (true)
	{
		if (program.textX == 0)
			return;
		
		if (isWhiteSpace(rows[program.textY][program.textX - 1]))
			return;

		if (isAtText)
		{
			if (!isText(rows[program.textY][program.textX - 1]))
				return;
		}
		else
		{
			if (isText(rows[program.textY][program.textX- 1]))
				return;
		}

		program.textX--;
	}
}

void TextEditor::MoveRightCtrl()
{
	bool isAtText;

	if (program.textX == rows[program.textY].size())
	{
		if (program.textY == rows.size() - 1)
			return;

		program.textY++;
		MoveHome();
		return;
	}

	while (true)
	{
		if (program.textX == rows[program.textY].size())
			return;

		if (isWhiteSpace(rows[program.textY][program.textX++]))
			continue;

		isAtText = isText(rows[program.textY][--program.textX]);
		break;
	}

	while (true)
	{
		if (program.textX == rows[program.textY].size())
			return;

		if (isWhiteSpace(rows[program.textY][program.textX]))
			return;

		if (isAtText)
		{
			if (!isText(rows[program.textY][program.textX]))
				return;
		}
		else
		{
			if (isText(rows[program.textY][program.textX]))
				return;
		}

		program.textX++;
	}
}

inline bool TextEditor::isWhiteSpace(char c)
{
	return c == ' ';
}

inline bool TextEditor::isText(char c)
{
	return std::isalpha(c) != 0;
}