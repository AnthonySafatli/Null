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
	int offset = GetCharIndex() * 4;

	TexCoords texCoords = GetCoords(ch);

	program.vertices.insert(program.vertices.begin() + offset++, Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , program.textY + 1, program.textX + 1, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , program.textY + 1, program.textX + 1, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), program.textY + 1, program.textX + 1, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), program.textY + 1, program.textX + 1, 0.0));

	int lastIndexInRowVertices = (GetLastIndexInRow() + 1) * 4;
	for (int i = offset; i < lastIndexInRowVertices; i++)
		program.vertices[i].column++;

	program.UpdateIndices();

	rows[program.textY].insert(rows[program.textY].begin() + program.textX, ch);

	MoveRight();

	program.SetData();
}

void TextArea::RemoveCharacterFromLeft()
{
	if (program.textX == 0)
	{
		if (program.textY == 0)
			return;

		int size = rows[program.textY - 1].size();
		int offset = GetCharIndex() * 4;

		rows[program.textY - 1] += rows[program.textY];
		rows.erase(rows.begin() + program.textY);

		MoveUp();

		int lastIndexInRowVertices = GetLastIndexInRow() * 4;
		for (int i = offset; i < lastIndexInRowVertices; i++)
		{
			program.vertices[i].row--;
			program.vertices[i].column += size;
		}

		program.textX = size;
		RemoveLeftMargin();
		program.SetData();
		return;
	}

	int offset = GetCharIndex() * 4 - 4;

	program.vertices.erase(program.vertices.begin() + offset, program.vertices.begin() + offset + 4);

	int lastIndexInRowVertices = (GetLastIndexInRow() - 1) * 4;
	for (int i = offset; i < lastIndexInRowVertices; i++)
		program.vertices[i].column--;

	program.indices.resize(program.indices.size() - 6);

	rows[program.textY].erase(rows[program.textY].begin() + program.textX - 1);

	MoveLeft();

	program.SetData();
}

void TextArea::RemoveCharacterFromRight()
{
	if (program.textX == rows[program.textY].size())
	{
		if (program.textY == rows.size() - 1)
			return;

		int size = rows[program.textY].size();

		rows[program.textY] += rows[program.textY + 1];
		rows.erase(rows.begin() + program.textY + 1);

		int j = 0;
		int offset = GetCharIndex() * 4;
		int lastIndexInRowVertices = GetLastIndexInRow() * 4;
		for (int i = offset; i < lastIndexInRowVertices; i++)
		{
			program.vertices[i].row--;
			program.vertices[i].column = size + (j / 4) + 1;
			j++;
		}

		program.SetData();
		return;
	}

	int offset = GetCharIndex() * 4;

	program.vertices.erase(program.vertices.begin() + offset, program.vertices.begin() + offset + 4);

	int lastIndexInRowVertices = GetLastIndexInRow() * 4 - 4;
	for (int i = offset; i < lastIndexInRowVertices; i++)
		program.vertices[i].column--;
 
	program.indices.resize(program.indices.size() - 6);

	rows[program.textY].erase(rows[program.textY].begin() + program.textX);

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
	program.textX = 0;
	MoveDown();
	
	AddLeftMargin();

	// Add characters back
	for (char ch : letters) AddCharacter(ch);
	MoveHome();
}

void TextArea::AddCharacterToMargin(const char ch, const int index)
{
	TexCoords texCoords = GetCoords(ch);

	program.marginVertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , program.textY + 1, index, 0.0));
	program.marginVertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , program.textY + 1, index, 0.0));
	program.marginVertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), program.textY + 1, index, 0.0));
	program.marginVertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), program.textY + 1, index, 0.0));

	program.UpdateIndices();

	program.SetData();
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
