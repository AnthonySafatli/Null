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
private:
	void Render();

public:
	StatusBar();

	void Init(const std::string initialMessage);

	void UpdateLength();

	void UpdateMessage(const std::string message);

	TextRow row;
	std::string message;
};