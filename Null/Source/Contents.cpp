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

	// std::cout << key << std::endl;

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
	{
		cursor.Move(RIGHT);
	}
	else if (key == KEYCODE_LEFT)
	{
		cursor.Move(LEFT);
	}
	else if (key == KEYCODE_UP)
	{
		cursor.Move(UP);
	}
	else if (key == KEYCODE_DOWN)
	{
		cursor.Move(DOWN);
	}

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
	// TODO: Stuff for AddCharacter()
	// use cursor controller to find which row cursor is on
	// use text.size() for all rows above cursor to see how many characters to skip
	// use column in cursor controller to see how many more characters
	// multiply by 4 to get how many vertices to skip
	// add vertices at that index
	// edit any vertices on same row that come after
	// add character to proper TextRow

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
	offset += cursor.x * 4;

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
	SaveChar(' ');
	cursor.Move(RIGHT);
}

void Contents::AddTab()
{
	for (int i = 0; i < tabAmount; i++)
		AddSpace();
}

void Contents::RemoveCharacter(bool left)
{
	if (left)
	{
		// TODO: Implement 'Del'
	}
	else
	{
		// TODO: Implement 'Delete'
	}
}

void Contents::Return()
{
	// TODO: Execute command if on CommandRow
	
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