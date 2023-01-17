#pragma once

#include "GLAbstraction.h"
#include "TextRow.h"

#include <vector>
#include <string>

class CommandRow
{
public:
	CommandRow();

	TextRow row;
};

class StatusBar
{
public:
	StatusBar();

	void Init(const unsigned int length, const std::string initialMessage);

	void UpdateLength(const unsigned int length);

	TextRow row;
};