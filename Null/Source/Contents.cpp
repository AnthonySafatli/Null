#include "Headers/Contents.h"
#include "Headers/Character.h"

#include "GLFW/glfw3.h"
#include "Headers/Uniforms.h"

#include <iostream>
#include <string>

/* ====== Contents ====== */

// CommandRow Contents::command = CommandRow();

Contents::Contents(const int width, const int height, const int tabAmount, const float textSize) 
	: idealWidth(800), idealHeight(1400), 
		width(width), height(height), tabAmount(tabAmount), textSize(textSize), rowIndex(0), columnIndex(0),
		indices(), currentScene(), cursor(0, 0),
		vertexBuffer(), indexBuffer(), vertexArray(), shaderProgram(), texture(),
		u_idealRatio(), u_size(), u_sceneRowIndex(), u_sceneColumnIndex(), u_tex()
{
	for (int i = 0; i < 2; i += 4)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back(i + 2);
		indices.push_back(i + 3);
		indices.push_back(i);
	}
}

void Contents::GLInit()
{
	vertexBuffer.Init();

	vertexArray.Init();
	VertexArray::EnableAttributes();

	indexBuffer.Init();

	shaderProgram.Init(
		"C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\vertex.glsl",
		"C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Shaders\\fragment.glsl"
	);

	texture.Init("C:\\Users\\Anthony\\source\\repos\\Null\\Null\\Resources\\Textures\\font texture.png");

	u_idealRatio.Init(shaderProgram.handle, "idealRatio");
	u_size.Init(shaderProgram.handle, "size");
	u_sceneRowIndex.Init(shaderProgram.handle, "sceneRowIndex");
	u_sceneColumnIndex.Init(shaderProgram.handle, "sceneColumnIndex");
	u_tex.Init(shaderProgram.handle, "tex");
}

void Contents::SetData()
{
	std::vector<Vertex> vertices;

	vertices.insert(vertices.end(), command.text.vertices.begin(), command.text.vertices.end());

	for (TextRow row : currentScene.rows)
		vertices.insert(vertices.end(), row.vertices.begin(), row.vertices.end());

	vertexBuffer.SetData(vertices);
	indexBuffer.SetData(indices);
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

	UpdateUniform2f(u_idealRatio.location, (float) idealWidth / (float) width, (float) idealHeight / (float) height);

	this->width = width;
	this->height = height;
}

void Contents::IncrementBarrier()
{
	for (TextRow row : currentScene.rows)
	{
		cursor.textX = 0;
		AddCharacter(' ');
	}

	cursor.sceneLeftBarrier++;
}

void Contents::AddCharacter(char ch)
{
	// Get vertex offset
	int offset = cursor.textX * 4;

	// Add vertices
	TexCoords texCoords = GetCoords(ch);

	currentScene.rows[cursor.textY].vertices.insert(currentScene.rows[cursor.textY].vertices.begin() + offset++,    // Add Vertex 1
		Vertex(0.0, 0.0, texCoords.u, texCoords.v, cursor.textY, cursor.textX, 0.0));
	currentScene.rows[cursor.textY].vertices.insert(currentScene.rows[cursor.textY].vertices.begin() + offset++,    // Add Vertex 2
		Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v, cursor.textY, cursor.textX, 0.0));
	currentScene.rows[cursor.textY].vertices.insert(currentScene.rows[cursor.textY].vertices.begin() + offset++,    // Add Vertex 3
		Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), cursor.textY, cursor.textX, 0.0)); 
	currentScene.rows[cursor.textY].vertices.insert(currentScene.rows[cursor.textY].vertices.begin() + offset++,    // Add Vertex 4
		Vertex(0.0, 1.0, texCoords.u, texCoords.v + (1.0 / 10.0), cursor.textY, cursor.textX, 0.0));                


	// Edit the vertices after
	for (int i = offset / 4 - 1; i < currentScene.rows[cursor.textY].text.size(); i++)
	{
		currentScene.rows[cursor.textY].vertices[offset++].column++;
		currentScene.rows[cursor.textY].vertices[offset++].column++;
		currentScene.rows[cursor.textY].vertices[offset++].column++;
		currentScene.rows[cursor.textY].vertices[offset++].column++;
	}

	// Add indices
	int startIndex = indices.size() / 6 * 4;
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

	// Update OpenGL
	SetData();
}

void Contents::AddTab()
{
	for (int i = 0; i < tabAmount; i++)
		AddCharacter(' ');
}

void Contents::RemoveCharacterFromLeft()
{
	// Delete Row
	if (cursor.textX <= cursor.sceneLeftBarrier)
	{
		cursor.textY--;
		cursor.textX = currentScene.rows[cursor.textY].text.size();

		int size = currentScene.rows[cursor.textY + 1].text.size();

		for (int i = cursor.sceneLeftBarrier; i < size; i++)
			AddCharacter(currentScene.rows[cursor.textY + 1].text[i]);

		currentScene.rows.erase(currentScene.rows.begin() + cursor.textY + 1);

		for (int i = 0; i < size * 6; i++) indices.pop_back();

		cursor.textX = currentScene.rows[cursor.textY].text.size() - size + cursor.sceneLeftBarrier;

		SetData();

		return;
	}

	// Calculate offset
	int offset = (cursor.textX - 1) * 4;

	// Remove vertices
	currentScene.rows[cursor.textY].vertices.erase(currentScene.rows[cursor.textY].vertices.begin() + offset, currentScene.rows[cursor.textY].vertices.begin() + offset + 4);

	while (offset < currentScene.rows[cursor.textY].vertices.size())
		currentScene.rows[cursor.textY].vertices[offset++].column--;

	// Remove indices
	for (int i = 0; i < 6; i++) indices.pop_back();

	// Remove letter from memory
	currentScene.rows[cursor.textY].text.erase(currentScene.rows[cursor.textY].text.begin() + cursor.textX - 1);

	// Move cursor
	cursor.Move(LEFT);

	// Update OpenGL
	SetData();
}

void Contents::RemoveCharacterFromRight()
{
	// Delete row
	if (cursor.textX >= currentScene.rows[cursor.textY].text.size())
	{
		int size = currentScene.rows[cursor.textY + 1].text.size();

		for (int i = cursor.sceneLeftBarrier; i < size; i++)
			AddCharacter(currentScene.rows[cursor.textY + 1].text[i]);

		currentScene.rows.erase(currentScene.rows.begin() + cursor.textY + 1);

		for (int i = 0; i < size * 6; i++) indices.pop_back();

		cursor.textX = currentScene.rows[cursor.textY].text.size() - size + cursor.sceneLeftBarrier;

		SetData();

		return; 
	}


	// Calculate vertex offset
	int offset = cursor.textX * 4;

	// Remove vertices
	currentScene.rows[cursor.textY].vertices.erase(currentScene.rows[cursor.textY].vertices.begin() + offset, currentScene.rows[cursor.textY].vertices.begin() + offset + 4);

	// Edit vertices after
	while (offset < currentScene.rows[cursor.textY].vertices.size())
		currentScene.rows[cursor.textY].vertices[offset++].column--;

	// Remove indices
	for (int i = 0; i < 6; i++) indices.pop_back();

	// Remove letter from memory
	currentScene.rows[cursor.textY].text.erase(currentScene.rows[cursor.textY].text.begin() + cursor.textY);

	// Update OpenGl
	SetData();
}

void Contents::Return()
{
	// TODO: Execute command if on CommandRow
	
	// Delete characters after cursor
	std::vector<char> letters(currentScene.rows[cursor.textY].text.begin() + cursor.textX, currentScene.rows[cursor.textY].text.end());

	int size = currentScene.rows[cursor.textY].text.size();
	for (int i = cursor.textX; i < size; i++) 
		RemoveCharacterFromRight();

	// Return
	currentScene.Return();

	// Add characters back
	for (char ch : letters) AddCharacter(ch);

	cursor.textX = cursor.sceneLeftBarrier;
}

void Contents::SaveChar(char ch)
{
	if (currentScene.rows[cursor.textY].text.size() == 0)
		currentScene.rows[cursor.textY].text.push_back(ch);
	else
		currentScene.rows[cursor.textY].text.insert(currentScene.rows[cursor.textY].text.begin() + cursor.textX, ch);
}