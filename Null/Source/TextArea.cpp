#include "Headers/TextArea.h"

#include "Headers/Program.h"
#include "Headers/Character.h"
#include "Headers/Uniforms.h"

extern Program program;

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
	int lastIndexInRow = GetLastIndexInRow() + 1;
	for (int i = offset; i < lastIndexInRow * 4; i++)
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
}

void TextArea::RemoveCharacterFromRight()
{
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
}

void TextArea::MoveUp()
{
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
}

void TextArea::MoveEnd()
{
}
