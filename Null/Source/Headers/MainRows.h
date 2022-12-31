#pragma once

#include "GLAbstraction.h"
#include "TextRow.h"

#include <vector>

class CommandRow
{
public:
	CommandRow();

	std::vector<Vertex> GetVertices();

	TextRow text;
};

class StatusBar
{
public:
	StatusBar();

	std::vector<Vertex> GetVertices();

	TextRow text;
};