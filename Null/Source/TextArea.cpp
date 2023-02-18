#include "Headers/TextArea.h"

#include "Headers/Program.h"
#include "Headers/Character.h"

extern Program program;

void TextArea::IncrementBarrier()
{
}

int TextArea::GetCharIndex()
{
	int index = program.textX;

	for (int i = 0; i < program.textY; i++)
		index += rows[program.textY].size();

	return index;
}

void TextArea::AddCharacter(const char ch)
{
	// Get column offset
	int offset = GetCharIndex();

	// Add vertices
	TexCoords texCoords = GetCoords(ch);

	program.vertices.insert(program.vertices.begin() + offset++, Vertex(0.0, 0.0, texCoords.u, texCoords.v, program.textY, program.textX, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v, program.textY, program.textX, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), program.textY, program.textX, 0.0));
	program.vertices.insert(program.vertices.begin() + offset++, Vertex(0.0, 1.0, texCoords.u, texCoords.v + (1.0 / 10.0), program.textY, program.textX, 0.0));

	
	// Edit the vertices after
	//for (int i = offset / 4 - 1; i < rows[program.textY].size(); i++)
	{
		//currentScene.rows[cursor.textY].vertices[offset++].column++;
		//currentScene.rows[cursor.textY].vertices[offset++].column++;
		//currentScene.rows[cursor.textY].vertices[offset++].column++;
		//currentScene.rows[cursor.textY].vertices[offset++].column++;
	}

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
}

void TextArea::Return()
{
}

void TextArea::MoveUp()
{
}

void TextArea::MoveDown()
{
}

void TextArea::MoveRight()
{
}

void TextArea::MoveLeft()
{
}

void TextArea::MoveHome()
{
}

void TextArea::MoveEnd()
{
}
