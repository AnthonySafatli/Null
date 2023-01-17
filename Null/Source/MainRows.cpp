#include "Headers/MainRows.h"
#include "Headers/Character.h"
#include "Headers/NullEditor.h"

#include <string>

/* ====== CommandRow ====== */

extern NullEditor appContents;

CommandRow::CommandRow() : row()
{
	TexCoords texCoords = GetCoords('>');

	row.vertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , -1, 1, 0));
	row.vertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , -1, 1, 0));
	row.vertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), -1, 1, 0));
	row.vertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), -1, 1, 0));
}

/* ====== StatusBar ====== */

StatusBar::StatusBar() : row()
{
}

void StatusBar::Init(const unsigned int length, const std::string initialMessage)
{
	// float rowPos = (1.0 / (0.1 * appContents.textSize)) * ((float)appContents.height / (float)appContents.idealHeight) - 3.5;
	float rowPos = -2;

	TexCoords texCoords = GetCoords(' ');

	row.vertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , rowPos, 0, 1));
	row.vertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , rowPos, 0, 1));
	row.vertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), rowPos, 0, 1));
	row.vertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), rowPos, 0, 1));

	for (int i = 1; i < length; i++)
	{
		if (i - 1 < initialMessage.size())
		{
			TexCoords texCoords = GetCoords(initialMessage.at(i - 1));

			row.vertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , rowPos, i, 1));
			row.vertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , rowPos, i, 1));
			row.vertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), rowPos, i, 1));
			row.vertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), rowPos, i, 1));
		}
		else
		{
			TexCoords texCoords = GetCoords(' ');

			row.vertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , rowPos, i, 1));
			row.vertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , rowPos, i, 1));
			row.vertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), rowPos, i, 1));
			row.vertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), rowPos, i, 1));
		}
	}
}

void StatusBar::UpdateLength(const unsigned int length)
{
}
