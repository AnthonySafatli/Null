#include "Headers/Program.h"

#include "GLFW/glfw3.h"

#include "Headers/TextEditor.h"
#include "Headers/TextArea.h"
#include "Headers/GLAbstraction.h"
#include "Headers/Uniforms.h"

Program::Program(const int height, const int width, const int textSize, const int tabAmount) 
	: height(height), width(width), textSize(textSize), tabAmount(tabAmount), 
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

void Program::ProcessKey(int key, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	area->ProcessKey(key, action, mods);
}

void Program::ProcessChar(unsigned int codepoint)
{
	if (!(codepoint > 31 && codepoint < 128))
		return;

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
