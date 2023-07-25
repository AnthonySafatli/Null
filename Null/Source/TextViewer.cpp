#include "Headers/TextViewer.h"

#include <string>

#include "Headers/Program.h"
#include "Headers/Character.h"

extern Program program;

TextViewer::TextViewer(const std::string text, const std::string pageName, AreaType type)
{
	this->type = type;

	ConstructorStart(2, false);

	UpdateText(text);

	ConstructorEnd();

	// RS: 8
	program.RenderStatus(pageName + " Loaded Successfully");
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