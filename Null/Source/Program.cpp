#include "Headers/Program.h"

#include "GLFW/glfw3.h"

#include "Headers/TextEditor.h"
#include "Headers/TextArea.h"
#include "Headers/GLAbstraction.h"
#include "Headers/Uniforms.h"

Program::Program(const int width, const int height, const float textSize, const int tabAmount) 
	: idealWidth(IDEAL_WIDTH), idealHeight(IDEAL_HEIGHT),
	  height(height), width(width), textSize(textSize), tabAmount(tabAmount), showCursor(false),
	  rowIndex(0), columnIndex(0), screenX(0), screenY(0), textX(0), textY(0), commandSelected(false)
{
	area = new TextEditor();
}

Program::~Program()
{
	delete area;
}

void Program::GLInit()
{
	openGL.Init();
}

void Program::SetData()
{
	// TODO: Change method to accept pointer
	openGL.vertexBuffer.SetData(vertices);
	openGL.indexBuffer.SetData(indices);
}

void Program::UpdateIndices()
{
	int neededIndices = vertices.size() / 4 * 6;

	int count = (neededIndices - indices.size()) / 6;

	int startIndex = indices.size() / 6 * 4;
	for (int i = 0; i < count; i++)
	{
		indices.push_back(startIndex);
		indices.push_back(startIndex + 1);
		indices.push_back(startIndex + 2);
		indices.push_back(startIndex + 2);
		indices.push_back(startIndex + 3);
		indices.push_back(startIndex);
		startIndex += 4;
	}
}

void Program::ProcessKey(int key, int action, int mods)
{
	area->ProcessKey(key, action, mods);
}

void Program::ProcessChar(unsigned int codepoint)
{
	if (commandSelected)
		// TODO: Implement Command Typing
		return;

	area->ProcessChar(codepoint);
}

void Program::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	UpdateUniform2f(openGL.u_idealRatio.location, (float)IDEAL_WIDTH / (float)width, (float)IDEAL_HEIGHT / (float)height);

	this->width = width;
	this->height = height;

	// TODO: Update status length

	SetData();

	area->OnResize(width, height);
}
