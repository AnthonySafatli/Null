#include "Headers/Program.h"

#include "GLFW/glfw3.h"

#include "Headers/TextEditor.h"
#include "Headers/TextArea.h"
#include "Headers/GLAbstraction.h"
#include "Headers/Uniforms.h"
#include "Headers/Character.h"

Program::Program(const int width, const int height, const float textSize, const int tabAmount) 
	: idealWidth(IDEAL_WIDTH), idealHeight(IDEAL_HEIGHT),
	  height(height), width(width), textSize(textSize), tabAmount(tabAmount), showCursor(false),
	  rowIndex(0), columnIndex(0), textX(0), textY(0), commandX(0), commandSelected(false)
{
	area = new TextEditor();

	TexCoords texCoords = GetCoords('>');

	commandVertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , -1, -2, 0.0));
	commandVertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , -1, -2, 0.0));
	commandVertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), -1, -2, 0.0));
	commandVertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), -1, -2, 0.0));
}

Program::~Program()
{
	delete area;
}

void Program::GLInit()
{
	openGL.Init();
	UpdateIndices();
	SetData();
}

void Program::SetData()
{
	// TODO: Change method to accept pointer

	std::vector<Vertex> all = vertices;
	all.insert(all.end(), marginVertices.begin(), marginVertices.end());
	all.insert(all.end(), commandVertices.begin(), commandVertices.end());

	openGL.vertexBuffer.SetData(all);
	openGL.indexBuffer.SetData(indices);
}

void Program::UpdateIndices()
{
	int neededIndices = (vertices.size() + marginVertices.size() + commandVertices.size()) / 4 * 6;

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
	if (key == KEYCODE_ESCAPE && action != GLFW_RELEASE)
	{
		commandSelected = !commandSelected;
		return;
	}

	if (commandSelected)
	{
		ProcessKeyCommand(key, action, mods);
		return;
	}

	area->ProcessKey(key, action, mods);
}

void Program::ProcessChar(unsigned int codepoint)
{
	if (commandSelected)
	{
		ProcessCharCommand(codepoint);
		return;
	}

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

void Program::ProcessKeyCommand(int key, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	switch (key)
	{
	case KEYCODE_HOME:
		MoveHomeCommand();
		break;
	case KEYCODE_END:
		MoveEndCommand();
		break;
	case KEYCODE_RIGHT:
		MoveRightCommand();
		break;
	case KEYCODE_LEFT:
		MoveLeftCommand();
		break;

	case KEYCODE_TAB:
		ToggleAutoComplete();
		break;
	case KEYCODE_UP:
		ScrollUpAutoComplete();
		break;
	case KEYCODE_DOWN:
		ScrollDownAutoComplete();
		break;

	case KEYCODE_ENTER:
		EnterCommand();
		break;

	case KEYCODE_DEL:
		RemoveCharacterFromLeftCommand();
		break;
	case KEYCODE_DELETE:
		RemoveCharacterFromRightCommand();
		break;
	}
}

void Program::ProcessCharCommand(unsigned int codepoint)
{
	if (!(codepoint > 31 && codepoint < 128))
		return;

	AddCharacterCommand((char)codepoint);
}

void Program::AddCharacterCommand(const char ch)
{
	int offset = commandX * 4 + 4;

	TexCoords texCoords = GetCoords(ch);

	commandVertices.insert(commandVertices.begin() + offset++, Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , -1, commandX + 1, 0.0));
	commandVertices.insert(commandVertices.begin() + offset++, Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , -1, commandX + 1, 0.0));
	commandVertices.insert(commandVertices.begin() + offset++, Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), -1, commandX + 1, 0.0));
	commandVertices.insert(commandVertices.begin() + offset++, Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), -1, commandX + 1, 0.0));

	for (int i = offset; i < commandVertices.size(); i++)
		commandVertices[i].column++;

	UpdateIndices();

	commandText.insert(commandText.begin() + commandX, ch);

	MoveRightCommand();

	SetData();
}

void Program::MoveHomeCommand()
{
}

void Program::MoveEndCommand()
{
}

void Program::MoveRightCommand()
{
	commandX++;
}

void Program::MoveLeftCommand()
{
}

void Program::ToggleAutoComplete()
{
}

void Program::ScrollUpAutoComplete()
{
}

void Program::ScrollDownAutoComplete()
{
}

void Program::EnterCommand()
{
}

void Program::RemoveCharacterFromLeftCommand()
{
}

void Program::RemoveCharacterFromRightCommand()
{
}
