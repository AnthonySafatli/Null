#include "Headers/Contents.h"
#include "Headers/Character.h"

#include "GLFW/glfw3.h"

#include <iostream>

/* ====== Contents ====== */

float Contents::textSize = 0.5;
CursorController Contents::cursor = CursorController(0, 0);
int Contents::tabAmount = 4;
std::vector<Vertex> Contents::vertices;
std::vector<unsigned int> Contents::indices;

void Contents::ProcessKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	if (key == ENTER)
	{
		Return();
	}
	else if (key == DEL)
	{
		RemoveCharacter(true);
	}
	else if (key == DELETE)
	{
		RemoveCharacter(false);
	}
	else if (key == HOME)
	{
		// TODO: Implement 'Home'
	}
	else if (key == END)
	{
		// TODO: Implement 'End'
	}

	// TODO: Implement shortcuts
}

void Contents::ProcessChar(GLFWwindow* window, unsigned int codepoint)
{
	if (codepoint == SPACE)
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
	// add character to proper TextRow

	// Add vertices
	TexCoords texCoords = GetCoords(ch);

	vertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , cursor.screenY, cursor.screenX, 0.0));
	vertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , cursor.screenY, cursor.screenX, 0.0));
	vertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), cursor.screenY, cursor.screenX, 0.0));
	vertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), cursor.screenY, cursor.screenX, 0.0));

	// Add indices
	int startIndex = vertices.size() - 4;
	indices.push_back(startIndex);
	indices.push_back(startIndex + 1);
	indices.push_back(startIndex + 2);
	indices.push_back(startIndex + 2);
	indices.push_back(startIndex + 3);
	indices.push_back(startIndex);

	cursor.Move(RIGHT);

	// TODO: Add char to vector
}

void Contents::AddSpace()
{
	cursor.screenX++;
	// TODO: Add space to something
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

	cursor.screenY++;
	cursor.screenX = 0;
}
