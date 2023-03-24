#include "Headers/TextViewer.h"

#include <string>

#include "Headers/Program.h"
#include "Headers/Character.h"

extern Program program;

TextViewer::TextViewer(const std::string text, const std::string pageName)
{
	SetLeftMargin(2);
	rows.push_back(std::string());

	program.textX = 0;
	program.textY = 0;

	TexCoords tildaCoords = GetCoords('~');
	program.marginVertices.push_back(Vertex(0.0, 0.0, tildaCoords.u,                tildaCoords.v,                1, -1, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 0.0, tildaCoords.u + (1.0 / 10.0), tildaCoords.v,                1, -1, MARGIN));
	program.marginVertices.push_back(Vertex(1.0, 1.0, tildaCoords.u + (1.0 / 10.0), tildaCoords.v + (1.0 / 10.0), 1, -1, MARGIN));
	program.marginVertices.push_back(Vertex(0.0, 1.0, tildaCoords.u,                tildaCoords.v + (1.0 / 10.0), 1, -1, MARGIN));

	UpdateText(text);

	program.HideCursor();
	program.showCursor = false;

	program.RenderStatus(pageName + " Loaded Successfully");
}

void TextViewer::ProcessKey(int key, int action, int mods)
{
}

void TextViewer::ProcessChar(unsigned int codepoint)
{
}

void TextViewer::OnResize(int width, int height)
{
}

void TextViewer::AddLeftMargin()
{
	AddCharacterToMargin('~', -1);
}

void TextViewer::RemoveLeftMargin()
{
	program.marginVertices.resize(program.marginVertices.size() - (4 * (leftMargin - 1)));
	program.indices.resize(program.indices.size() - (6 * (leftMargin - 1)));
}

void TextViewer::UpdateText(const std::string text)
{
	for (char ch : text)
	{
		if (ch == '\n')
		{
			Return();
			continue;
		}

		AddCharacter(ch);
	}
}