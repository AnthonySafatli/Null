#pragma once

#include "GLAbstraction.h"

#include <vector>

class TextRow
{
public:
	TextRow();

	std::vector<Vertex> vertices;
	std::vector<char> text;
};