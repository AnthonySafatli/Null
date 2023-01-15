#include "Headers/MainRows.h"
#include "Headers/Character.h"

/* ====== CommandRow ====== */

CommandRow::CommandRow() : text()
{
	TexCoords texCoords = GetCoords('>');

	text.vertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , -1, 1, 0.0));
	text.vertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , -1, 1, 0.0));
	text.vertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), -1, 1, 0.0));
	text.vertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), -1, 1, 0.0));
}

/* ====== StatusBar ====== */

StatusBar::StatusBar() : text()
{
}
