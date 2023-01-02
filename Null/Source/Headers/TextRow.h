#pragma once

#include "GLAbstraction.h"

#include <vector>

class TextRow
{
public:
	TextRow();

	std::vector<Vertex> GetVertices(const float rowNumber, const float startLetterPos);

	std::vector<char> text;

	// create static way to search for texture coords for a letter
};