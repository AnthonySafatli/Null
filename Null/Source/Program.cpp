#include "Headers/Program.h"

Program::Program(const int height, const int width, const int textSize, const int tabAmount) 
	: height(height), width(width), textSize(textSize), tabAmount(tabAmount)
	// TODO: Initialize all members
{
}

void Program::ProcessKey(int key, int action, int mods)
{
	area->OnKey();
}

void Program::ProcessChar(unsigned int codepoint)
{
	area->OnChar();
}

void Program::OnResize(int width, int height)
{
	area->OnResize();
}
