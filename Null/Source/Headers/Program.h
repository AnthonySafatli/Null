#pragma once

#include "TextArea.h"
#include "GLObejct.h"
#include "GLAbstraction.h"

#include <vector>
#include <string>

#define IDEAL_WIDTH 800
#define IDEAL_HEIGHT 1400

class Program
{
public:
	/* Constructor */
	Program(const int width, const int height, const float textSize, const int tabAmount);
	~Program();

	/* OpenGL */
	GLObject openGL;
	void GLInit();
	void SetData();

	/* Callback */
	void ProcessKey(int key, int action, int mods);
	void ProcessChar(unsigned int codepoint);
	void OnResize(int width, int height);

	/* Vertex Data */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	void UpdateIndices();

	/* Text Data */
	std::string commandText;
	TextArea *area;
	std::string statusText;

	/* Text Cursor */
	unsigned int textX;
	unsigned int textY;
	unsigned int screenX;
	unsigned int screenY;
	unsigned int rowIndex;
	unsigned int columnIndex;

	bool commandSelected;

	/* Text Properties */
	bool showCursor;
	int tabAmount;
	float textSize;

	/* Window Properties */
	int width;
	int height;
	const int idealWidth;
	const int idealHeight;
};