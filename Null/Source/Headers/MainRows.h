#pragma once

#include "GLAbstraction.h"
#include "TextRow.h"

#include <vector>

class CommandRow
{
public:
	CommandRow();

	TextRow text;
};

class StatusBar
{
public:
	StatusBar();

	TextRow text;
};