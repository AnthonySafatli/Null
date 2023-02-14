#pragma once

#include "TextArea.h"
#include "GLObejct.h"
#include "GLAbstraction.h"

#include <vector>

class Program
{
public:
	/* Constructor */
	Program(const int height, const int width, const int textSize, const int tabAmount);

	/* Callback */
	void ProcessKey(int key, int action, int mods);
	void ProcessChar(unsigned int codepoint);
	void OnResize(int width, int height);

	/* Vertex Data */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	/* Members */
	TextArea *area;
	GLObject openGL;

	/* Text Cursor */
	unsigned int textX;
	unsigned int textY;
	unsigned int screenX;
	unsigned int screenY;
	unsigned int rowIndex;
	unsigned int columnIndex;

	bool commandSelected;

	/* Text Properties */
	int tabAmount;
	float textSize;

	/* Window Properties */
	int width;
	int height;
};