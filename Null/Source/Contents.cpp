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

void Contents::ProcessKey(int key, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	if (key == KEYCODE_TAB)
		AddTab();
	else if (key == KEYCODE_ENTER)
		Return();
	else if (key == KEYCODE_DEL)
		RemoveCharacterFromLeft();
	else if (key == KEYCODE_DELETE)
		RemoveCharacterFromRight();

	else if (key == KEYCODE_HOME)
		cursor.Move(HOME);
	else if (key == KEYCODE_END)
		cursor.Move(END);

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

void Contents::ProcessChar(unsigned int codepoint)
{
	if (codepoint > 31 && codepoint < 128)
		AddCharacter(codepoint);
}

void Contents::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	// TODO: Change idealRatio uniform (glUniform2f(u_idealRatio.location, idealWidth / width, idealHeight / height);)
}

void Contents::AddCharacter(char ch)
{
	// Get vertex offset
	int offset = 0;
	
	for (int i = 0; i < cursor.y; i++)
		offset += currentScene.text[i].text.size();

	offset += cursor.x;

	offset *= 4;

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
	cursor.Move(RIGHT);
}

void Contents::AddTab()
{
	for (int i = 0; i < tabAmount; i++)
		AddCharacter(' ');
}

void Contents::RemoveCharacterFromLeft()
{
	// Error check
	if (cursor.x <= 0)
		return;

	// Calculate vertex offset
	int offset = 0;

	for (int i = 0; i < cursor.y; i++)
		offset += currentScene.text[i].text.size();

	offset += cursor.x - 1;

	offset *= 4;

	// Remove vertices
	vertices.erase(vertices.begin() + offset, vertices.begin() + offset + 4);

	// Edit vertices after
	for (int i = 0; i < (currentScene.text[cursor.y].text.size() - cursor.x) * 4; i++)
		vertices[offset++].column--;

	// Remove indices
	for (int i = 0; i < 6; i++) indices.pop_back();

	// Remove letter from memory
	currentScene.text[cursor.y].text.erase(currentScene.text[cursor.y].text.begin() + cursor.x - 1);

	// Move cursor
	cursor.Move(LEFT);
}

void Contents::RemoveCharacterFromRight()
{
	// Error check
	if (cursor.x >= currentScene.text[cursor.y].text.size())
		return;

	// Calculate vertex offset
	int offset = 0;

	for (int i = 0; i < cursor.y; i++)
		offset += currentScene.text[i].text.size();

	offset += cursor.x;

	offset *= 4;

	// Remove vertices
	vertices.erase(vertices.begin() + offset, vertices.begin() + offset + 4);

	// Edit vertices after
	for (int i = 0; i < (currentScene.text[cursor.y].text.size() - cursor.x - 4) * 4; i++)
		vertices[offset++].column--;

	// Remove indices
	for (int i = 0; i < 6; i++) indices.pop_back();

	// Remove letter from memory
	currentScene.text[cursor.y].text.erase(currentScene.text[cursor.y].text.begin() + cursor.x);
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