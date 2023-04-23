#pragma once

#include <vector>
#include <string>

struct UndoObject 
{
	UndoObject(std::vector<std::string> text, unsigned int x, unsigned int y);

	std::vector<std::string> text;
	unsigned int textX;
	unsigned int textY;
};