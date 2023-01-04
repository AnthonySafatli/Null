#include "Headers/Contents.h"
#include "Headers/Character.h"

#include <iostream>

/* ====== Contents ====== */

Contents::Contents() : textSize(0.5), cursor(0, 0), charAmount(0), tabAmount(4)
{
	// Only run once at beginning of program
}

void Contents::AddCharacter(char ch)
{
	// use cursor controller to find which row cursor is on
	// use text.size() for all rows above cursor to see how many characters to skip
	// use column in cursor controller to see how many more characters
	// multiply by 4 to get how many vertices to skip
	// add vertices at that index
	// add character to proper TextRow
	
	//int rowNumber;

	// Add vertices
	TexCoords texCoords = GetCoords(ch);

	vertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , 0.0, charAmount));
	vertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , 0.0, charAmount));
	vertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), 0.0, charAmount));
	vertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), 0.0, charAmount));

	// Add indices
	int startIndex = vertices.size() - 4;
	indices.push_back(startIndex);
	indices.push_back(startIndex + 1);
	indices.push_back(startIndex + 2);
	indices.push_back(startIndex + 2);
	indices.push_back(startIndex + 3);
	indices.push_back(startIndex);

	charAmount++;
}

void Contents::AddSpace()
{
	charAmount++;
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
