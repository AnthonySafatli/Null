#include "Headers/TextArea.h"

#include <vector>
#include <string>

#include "Headers/Program.h"
#include "Headers/Character.h"
#include "Headers/Uniforms.h"

extern Program program;

/* ================= Text Methods ================= */

void TextArea::SetLeftMargin(const int margin)
{
	leftMargin = margin;
	UpdateUniform1i(program.openGL.u_leftMargin.location, margin);
}

int TextArea::GetCharIndex()
{
	int index = program.textX;

	for (int i = 0; i < program.textY; i++)
		index += rows[i].size();

	return index;
}

int TextArea::GetLastIndexInRow()
{
	int index = 0;

	for (int i = 0; i <= (int)program.textY; i++)
		index += rows[i].size();

	return index;
}

void TextArea::AddCharacter(const char ch)
{
	// Get column offset
	int offset = GetCharIndex() * 4;

	// Add vertices
	TexCoords texCoords = GetCoords(ch);

	program.vertices.insert(program.vertices.begin() + offset++, Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , program.textY + 1, program.textX + 1, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , program.textY + 1, program.textX + 1, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), program.textY + 1, program.textX + 1, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), program.textY + 1, program.textX + 1, 0.0));

	// Edit the vertices after
	int lastIndexInRowVertices = (GetLastIndexInRow() + 1) * 4;
	for (int i = offset; i < lastIndexInRowVertices; i++)
		program.vertices[i].column++;

	// Add indices
	program.UpdateIndices();

	// Add letter to memory
	rows[program.textY].insert(rows[program.textY].begin() + program.textX, ch);

	// Move cursor forwards
	MoveRight();

	// Update OpenGL
	program.SetData();
}

void TextArea::RemoveCharacterFromLeft()
{
	// Delete Row
	if (program.textX == 0)
	{
		if (program.textY == 0)
			return;

		int size = rows[program.textY - 1].size();
		int offset = GetCharIndex() * 4;

		// Move string data to row before
		rows[program.textY - 1] += rows[program.textY];
		rows.erase(rows.begin() + program.textY);

		// Move cursor up
		MoveUp();

		// Edit the vertices to row before
		int lastIndexInRowVertices = GetLastIndexInRow() * 4;
		for (int i = offset; i < lastIndexInRowVertices; i++)
		{
			program.vertices[i].row--;
			program.vertices[i].column += size;
		}

		// Move cursor
		program.textX = size;

		// Update OpenGL
		program.SetData();

		return;
	}

	// Get column offset
	int offset = GetCharIndex() * 4 - 4;

	// Remove vertices
	program.vertices.erase(program.vertices.begin() + offset, program.vertices.begin() + offset + 4);

	// Edit the vertices after
	int lastIndexInRowVertices = (GetLastIndexInRow() - 1) * 4;
	for (int i = offset; i < lastIndexInRowVertices; i++)
		program.vertices[i].column--;

	// Remove indices
	program.indices.resize(program.indices.size() - 6);

	// Remove letter to memory
	rows[program.textY].erase(rows[program.textY].begin() + program.textX - 1);

	// Move cursor back
	MoveLeft();

	// Update OpenGL
	program.SetData();
}

void TextArea::RemoveCharacterFromRight()
{
	// Delete Row
	if (program.textX == rows[program.textY].size())
	{
		if (program.textY == rows.size() - 1)
			return;

		int size = rows[program.textY].size();

		// Move string data to row before
		rows[program.textY] += rows[program.textY + 1];
		rows.erase(rows.begin() + program.textY + 1);

		// Edit the vertices to row above
		int j = 0;
		int offset = GetCharIndex() * 4;
		int lastIndexInRowVertices = GetLastIndexInRow() * 4;
		for (int i = offset; i < lastIndexInRowVertices; i++)
		{
			program.vertices[i].row--;
			program.vertices[i].column = size + (j / 4) + 1;
			j++;
		}

		// Update OpenGL
		program.SetData();

		return;
	}

	// Get column offset
	int offset = GetCharIndex() * 4;

	// Remove vertices
	program.vertices.erase(program.vertices.begin() + offset, program.vertices.begin() + offset + 4);

	// Edit the vertices after
	int lastIndexInRowVertices = GetLastIndexInRow() * 4 - 4;
	for (int i = offset; i < lastIndexInRowVertices; i++)
		program.vertices[i].column--;
 
	// Remove indices
	program.indices.resize(program.indices.size() - 6);

	// Remove letter to memory
	rows[program.textY].erase(rows[program.textY].begin() + program.textX);

	// Update OpenGL
	program.SetData();
}

void TextArea::AddTab()
{
	for (int i = 0; i < program.tabAmount; i++)
		AddCharacter(' ');
}

void TextArea::Return()
{
	// Delete characters after cursor
	std::vector<char> letters(rows[program.textY].begin() + program.textX, rows[program.textY].end());

	int size = rows[program.textY].size();
	while (program.textX < rows[program.textY].size())
	 	RemoveCharacterFromRight();

	// Add New Row
	rows.push_back(std::string());
	MoveDown();
	program.textX = 0;
	
	AddLeftMargin();

	// Add characters back
	for (char ch : letters) AddCharacter(ch);
	program.textX = 0;
}

void TextArea::AddCharacterToMargin(const char ch, const int index)
{
	/*
	// Get column offset
	int offset = GetCharIndex() * 4;

	// Add vertices
	TexCoords texCoords = GetCoords(ch);

	program.vertices.insert(program.vertices.begin() + offset++, Vertex(0.0, 0.0, texCoords.u, texCoords.v, program.textY + 1, program.textX + 1, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v, program.textY + 1, program.textX + 1, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), program.textY + 1, program.textX + 1, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(0.0, 1.0, texCoords.u, texCoords.v + (1.0 / 10.0), program.textY + 1, program.textX + 1, 0.0));

	// Edit the vertices after
	int lastIndexInRowVertices = (GetLastIndexInRow() + 1) * 4;
	for (int i = offset; i < lastIndexInRowVertices; i++)
		program.vertices[i].column++;

	// Add indices
	program.UpdateIndices();

	// Add letter to memory
	rows[program.textY].insert(rows[program.textY].begin() + program.textX, ch);

	// Move cursor forwards
	MoveRight();

	// Update OpenGL
	program.SetData();
	*/
}

/* ================= Movement Methods ================= */

void TextArea::MoveUp()
{
	// TODO: Remake but with scrolling in mind
	if (program.textY > 0)
		program.textY--;
}

void TextArea::MoveDown()
{
	// TODO: Remake but with scrolling in mind
	if (program.textY < rows.size() - 1)
		program.textY++;
}

void TextArea::MoveRight()
{
	if (program.textX < rows[program.textY].size())
		program.textX++;
}

void TextArea::MoveLeft()
{
	if (program.textX > 0)
		program.textX--;
}

void TextArea::MoveHome()
{
	program.textX = 0;
}

void TextArea::MoveEnd()
{
	program.textX = rows[program.textY].size();
}
