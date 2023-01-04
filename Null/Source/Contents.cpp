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

void Contents::ProcessKey(const int key, const int action, const int mods)
{
	std::cout << key << std::endl;
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
}

void Contents::AddSpace()
{
	// TODO: Add space to something
}

void Contents::AddTab()
{
	for (int i = 0; i < tabAmount; i++)
		AddSpace();
}

void Contents::RemoveCharacter()
{
	// same as adding but not
}

void Contents::Return()
{
	// if on command row, execute command
	// else create new text row
}
