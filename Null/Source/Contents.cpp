#include "Headers/Contents.h"
#include "Headers/Character.h"

#include "GLFW/glfw3.h"

#include <iostream>
#include <string>

/* ====== Contents ====== */

CommandRow Contents::command = CommandRow();
Scene Contents::currentScene = Scene();
CursorController Contents::cursor = CursorController(0, 0);

std::vector<Vertex> Contents::vertices;
std::vector<unsigned int> Contents::indices;

float Contents::textSize = 0.3;
int Contents::tabAmount = 4;

void Contents::ProcessKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	if (key == KEYCODE_TAB)
		AddTab();
	else if (key == KEYCODE_ENTER)
		Return();
	else if (key == KEYCODE_DEL)
		RemoveCharacter(true);
	else if (key == KEYCODE_DELETE)
		RemoveCharacter(false);

	else if (key == KEYCODE_HOME)
	{
		// TODO: Implement 'Home'
	}
	else if (key == KEYCODE_END)
	{
		// TODO: Implement 'End'
	}

	else if (key == KEYCODE_RIGHT)
		cursor.Move(RIGHT);
	else if (key == KEYCODE_LEFT)
		cursor.Move(LEFT);
	else if (key == KEYCODE_UP)
		cursor.Move(UP);
	else if (key == KEYCODE_DOWN)
		cursor.Move(DOWN);

	// TODO: Implement shortcuts
}

void Contents::ProcessChar(GLFWwindow* window, unsigned int codepoint)
{
	if (codepoint == KEYCODE_SPACE)
		AddSpace();

	else if (codepoint > 32 && codepoint < 128)
		AddCharacter(codepoint);
}

void Contents::OnResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	// TODO: glUniform2f();
}

void Contents::AddCharacter(char ch)
{
	int offset = 0;

	// Get offset from rows
	if (cursor.y > 0)
	{
		offset += (command.text.text.size() - command.text.whiteSpaceCount) * 4;

		for (int i = 0; i < cursor.y - 1; i++)
		{
			offset += (currentScene.text[i].text.size() - currentScene.text[i].whiteSpaceCount) * 4;
		}
	}

	// Get offset from columns
	int whiteSpaceCountSoFar = 0;

	if (cursor.y == 0)
	{
		for (int i = 0; i < cursor.x; i++)
		{
			if (command.text.text[i] == ' ')
				whiteSpaceCountSoFar++;
		}
	}
	else
	{
		for (int i = 0; i < cursor.x; i++)
		{
			if (currentScene.text[cursor.y - 1].text[i] == ' ')
				whiteSpaceCountSoFar++;
		}
	}

	offset += (cursor.x - whiteSpaceCountSoFar) * 4;

	// Add vertices
	TexCoords texCoords = GetCoords(ch);

	vertices.insert(vertices.begin() + offset++, Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , cursor.y - cursor.sceneStartIndex, cursor.x, 0.0));
	vertices.insert(vertices.begin() + offset++, Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , cursor.y - cursor.sceneStartIndex, cursor.x, 0.0));
	vertices.insert(vertices.begin() + offset++, Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), cursor.y - cursor.sceneStartIndex, cursor.x, 0.0));
	vertices.insert(vertices.begin() + offset++, Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), cursor.y - cursor.sceneStartIndex, cursor.x, 0.0));

	// Edit the vertices after
	int columnSize = cursor.y == 0 ? command.text.text.size() : currentScene.text[cursor.y - 1].text.size();

	for (int i = 0; i < columnSize - cursor.x; i++)
	{
		vertices[offset++].column++;
		vertices[offset++].column++;
		vertices[offset++].column++;
		vertices[offset++].column++;
	}

	// Add indices
	int startIndex = vertices.size() - 4;
	indices.push_back(startIndex);
	indices.push_back(startIndex + 1);
	indices.push_back(startIndex + 2);
	indices.push_back(startIndex + 2);
	indices.push_back(startIndex + 3);
	indices.push_back(startIndex);

	// Add letter to memory
	SaveChar(ch);

	// Move cursor forwards
	cursor.Move(RIGHT);
}

void Contents::AddSpace()
{
	int offset = 0;

	// Get offset from rows
	if (cursor.y > 0)
	{
		offset += (command.text.text.size() - command.text.whiteSpaceCount) * 4;

		for (int i = 0; i < cursor.y - 1; i++)
		{
			offset += (currentScene.text[i].text.size() - currentScene.text[i].whiteSpaceCount) * 4;
		}
	}

	// Get offset from columns
	int whiteSpaceCountSoFar = 0;

	if (cursor.y == 0)
	{
		for (int i = 0; i < cursor.x; i++)
		{
			if (command.text.text[i] == ' ')
				whiteSpaceCountSoFar++;
		}
	}
	else
	{
		for (int i = 0; i < cursor.x; i++)
		{
			if (currentScene.text[cursor.y - 1].text[i] == ' ')
				whiteSpaceCountSoFar++;
		}
	}

	offset += (cursor.x - whiteSpaceCountSoFar) * 4;

	// Edit the vertices after
	int columnSize = cursor.y == 0 ? command.text.text.size() : currentScene.text[cursor.y - 1].text.size();

	for (int i = 0; i < columnSize - cursor.x; i++)
	{
		vertices[offset++].column++;
		vertices[offset++].column++;
		vertices[offset++].column++;
		vertices[offset++].column++;
	}

	// Add letter to memory
	SaveChar(' ');

	// Increment whitespace counter
	if (cursor.y == 0)
		command.text.whiteSpaceCount++;
	else
		currentScene.text[cursor.y - 1].whiteSpaceCount++;

	// Move cursor forwards
	cursor.Move(RIGHT);
}

void Contents::AddTab()
{
	for (int i = 0; i < tabAmount; i++)
		AddSpace();
}

void Contents::RemoveCharacter(bool left)
{
	char characterToDelete;

	int cursorPos;

	if (left)
	{
		// Get character to delete
		if (cursor.x < 0)
			return;

		characterToDelete = cursor.y == 0 ? command.text.text[cursor.x - 1] : currentScene.text[cursor.y - 1].text[cursor.x - 1];

		cursorPos = cursor.x - 1;

	}
	else
	{
		// Get character to delete
		int charArraySize = cursor.y == 0 ? command.text.text.size() : currentScene.text[cursor.y - 1].text.size();

		if (cursor.x >= charArraySize)
			return;

		characterToDelete = cursor.y == 0 ? command.text.text[cursor.x] : currentScene.text[cursor.y - 1].text[cursor.x];

		cursorPos = cursor.x;
	}

	int offset = 0;

	// Remove vertices
	if (characterToDelete != ' ')
	{

		// Get offset from rows
		if (cursor.y > 0)
		{
			offset += (command.text.text.size() - command.text.whiteSpaceCount) * 4;

			for (int i = 0; i < cursor.y - 1; i++)
			{
				offset += (currentScene.text[i].text.size() - currentScene.text[i].whiteSpaceCount) * 4;
			}
		}

		// Get offset from columns
		int whiteSpaceCountSoFar = 0;

		if (cursor.y == 0)
		{
			for (int i = 0; i < cursorPos; i++)
			{
				if (command.text.text[i] == ' ')
					whiteSpaceCountSoFar++;
			}
		}
		else
		{
			for (int i = 0; i < cursorPos - 1; i++)
			{
				if (currentScene.text[cursor.y - 1].text[i] == ' ')
					whiteSpaceCountSoFar++;
			}
		}

		offset += (cursorPos - whiteSpaceCountSoFar) * 4;

		// Remove vertices
		vertices.erase(vertices.begin() + offset, vertices.begin() + offset + 4);
		for (int i = 0; i < 6; i++) indices.pop_back();
	}

	// Edit vertices after
	int columnSize = cursor.y == 0 ? command.text.text.size() : currentScene.text[cursor.y - 1].text.size();

	if (!left)
		cursor.Move(RIGHT);

	for (int i = 0; i < columnSize - cursor.x; i++)
	{
		vertices[offset++].column--;
		vertices[offset++].column--;
		vertices[offset++].column--;
		vertices[offset++].column--;
	}

	// Remove character from memory
	if (cursor.y == 0)
		command.text.text.erase(command.text.text.begin() + cursorPos);
	else
		currentScene.text[cursor.y - 1].text.erase(currentScene.text[cursor.y - 1].text.begin() + cursorPos);

	// Move cursor
	cursor.Move(LEFT);
}

void Contents::Return()
{
	// TODO: Execute command if on CommandRow
	
	currentScene.text.push_back(TextRow()); 
	cursor.y++;
	cursor.x = 0;
}

void Contents::SaveChar(char ch)
{
	if (cursor.y == 0)
	{
		if (command.text.text.size() == 0)
			command.text.text.push_back(ch);
		else
			command.text.text.insert(command.text.text.begin() + cursor.x, ch);
	}
	else
	{
		if (currentScene.text[cursor.y - 1].text.size() == 0)
			currentScene.text[cursor.y - 1].text.push_back(ch);
		else
			currentScene.text[cursor.y - 1].text.insert(currentScene.text[cursor.y - 1].text.begin() + cursor.x, ch);
	}
}