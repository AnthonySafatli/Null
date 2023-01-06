#include "Headers/Contents.h"
#include "Headers/Character.h"

#include "GLFW/glfw3.h"

#include <iostream>
#include <string>

/* ====== Contents ====== */

// CommandRow Contents::command = CommandRow();

Contents::Contents(const int width, const int height, const int tabAmount, const float textSize, const int startX, const int startY) 
	: idealWidth(800), idealHeight(1400), 
		width(width), height(height), tabAmount(tabAmount), textSize(textSize),
		vertices(), indices(), currentScene(), cursor(startX, startY)
{
}

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
		cursor.Move(currentScene, cursor.y, RIGHT);
	else if (key == KEYCODE_LEFT)
		cursor.Move(currentScene, cursor.y, LEFT);
	else if (key == KEYCODE_UP)
		cursor.Move(currentScene, cursor.y, UP);
	else if (key == KEYCODE_DOWN)
		cursor.Move(currentScene, cursor.y, DOWN);

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
	for (int i = 0; i < cursor.y; i++)
	{
		offset += (currentScene.text[i].text.size() - currentScene.text[i].whiteSpaceCount) * 4;
	}

	// Get offset from columns
	int whiteSpaceCountSoFar = 0;
	
	for (int i = 0; i < cursor.x; i++)
	{
		if (currentScene.text[cursor.y].text[i] == ' ')
			whiteSpaceCountSoFar++;
	}

	offset += (cursor.x - whiteSpaceCountSoFar) * 4;

	// Add vertices
	TexCoords texCoords = GetCoords(ch);

	vertices.insert(vertices.begin() + offset++, Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , cursor.y - cursor.sceneStartIndex, cursor.x, 0.0));
	vertices.insert(vertices.begin() + offset++, Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , cursor.y - cursor.sceneStartIndex, cursor.x, 0.0));
	vertices.insert(vertices.begin() + offset++, Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), cursor.y - cursor.sceneStartIndex, cursor.x, 0.0));
	vertices.insert(vertices.begin() + offset++, Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), cursor.y - cursor.sceneStartIndex, cursor.x, 0.0));

	// Edit the vertices after
	int columnSize = currentScene.text[cursor.y].text.size();

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
	cursor.Move(currentScene, cursor.y, RIGHT);
}

void Contents::AddSpace()
{
	int offset = 0;

	// Get offset from rows
	for (int i = 0; i < cursor.y; i++)
	{
		offset += (currentScene.text[i].text.size() - currentScene.text[i].whiteSpaceCount) * 4;
	}

	// Get offset from columns
	int whiteSpaceCountSoFar = 0;

	for (int i = 0; i < cursor.x; i++)
	{
		if (currentScene.text[cursor.y].text[i] == ' ')
			whiteSpaceCountSoFar++;
	}

	offset += (cursor.x - whiteSpaceCountSoFar) * 4;

	// Edit the vertices after
	int columnSize = currentScene.text[cursor.y].text.size();

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
	currentScene.text[cursor.y].whiteSpaceCount++;

	// Move cursor forwards
	cursor.Move(currentScene, cursor.y, RIGHT);
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

		characterToDelete = currentScene.text[cursor.y].text[cursor.x - 1];

		cursorPos = cursor.x - 1;
	}
	else
	{
		// Get character to delete
		int charArraySize = currentScene.text[cursor.y].text.size();

		if (cursor.x >= charArraySize)
			return;

		characterToDelete = currentScene.text[cursor.y].text[cursor.x];

		cursorPos = cursor.x;
	}

	int offset = 0;

	// Remove vertices
	if (characterToDelete != ' ')
	{
		// Get offset from rows
		for (int i = 0; i < cursor.y; i++)
		{
			offset += (currentScene.text[i].text.size() - currentScene.text[i].whiteSpaceCount) * 4;
		}

		// Get offset from columns
		int whiteSpaceCountSoFar = 0;

		for (int i = 0; i < cursorPos; i++)
		{
			if (currentScene.text[cursor.y].text[i] == ' ')
				whiteSpaceCountSoFar++;
		}

		offset += (cursorPos - whiteSpaceCountSoFar) * 4;

		// Remove vertices
		vertices.erase(vertices.begin() + offset, vertices.begin() + offset + 4);
		for (int i = 0; i < 6; i++) indices.pop_back();
	}

	// Edit vertices after
	int columnSize = currentScene.text[cursor.y].text.size();

	if (!left)
		cursor.Move(currentScene, cursor.y, RIGHT);

	for (int i = 0; i < columnSize - cursor.x; i++)
	{
		vertices[offset++].column--;
		vertices[offset++].column--;
		vertices[offset++].column--;
		vertices[offset++].column--;
	}

	// Remove character from memory
	currentScene.text[cursor.y].text.erase(currentScene.text[cursor.y].text.begin() + cursorPos);

	// Move cursor
	cursor.Move(currentScene, cursor.y, LEFT);
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
	if (currentScene.text[cursor.y].text.size() == 0)
		currentScene.text[cursor.y].text.push_back(ch);
	else
		currentScene.text[cursor.y].text.insert(currentScene.text[cursor.y].text.begin() + cursor.x, ch);
}