#include "Headers/TextRow.h"

/* ====== TextRow ====== */

TextRow::TextRow()
{

}

std::vector<Vertex> TextRow::GetVertices(const float rowNumber, const float startLetterPos = 0.0)
{
	std::vector<Vertex> vertices;

	for (int i = 0; i < text.size(); i++)
	{
		// get texture coord for letter
		vertices.push_back(Vertex( (i + startLetterPos) * 8.0     , (rowNumber - 1.0) * 23.0, 0.0, 0.0));
		vertices.push_back(Vertex(((i + startLetterPos) + 1) * 8.0, (rowNumber - 1.0) * 23.0, 0.0, 0.0));
		vertices.push_back(Vertex( (i + startLetterPos) * 8.0     , (rowNumber) * 23.0      , 0.0, 0.0));
		vertices.push_back(Vertex(((i + startLetterPos) + 1) * 8.0, (rowNumber) * 23.0      , 0.0, 0.0));
	}

	return vertices;
}
