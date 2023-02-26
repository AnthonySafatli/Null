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
	// Add > to Command Line
	TexCoords texCoordsCommand = GetCoords('>');

	commandVertices.push_back(Vertex(0.0, 0.0, texCoordsCommand.u               , texCoordsCommand.v               , -1, -2, 0.0));
	commandVertices.push_back(Vertex(1.0, 0.0, texCoordsCommand.u + (1.0 / 10.0), texCoordsCommand.v               , -1, -2, 0.0));
	commandVertices.push_back(Vertex(1.0, 1.0, texCoordsCommand.u + (1.0 / 10.0), texCoordsCommand.v + (1.0 / 10.0), -1, -2, 0.0));
	commandVertices.push_back(Vertex(0.0, 1.0, texCoordsCommand.u               , texCoordsCommand.v + (1.0 / 10.0), -1, -2, 0.0));

	// Create Status Bar
	float columnAmount = ((1.0 / (textSize * 0.1)) * ((float)width / (float)IDEAL_WIDTH));
	TexCoords texCoordsStatus = GetCoords(' ');

	for (int i = 0; i < columnAmount; i++)
	{
		statusVertices.push_back(Vertex(0.0, 0.0, texCoordsStatus.u               , texCoordsStatus.v               , -2, i, 1.0));
		statusVertices.push_back(Vertex(1.0, 0.0, texCoordsStatus.u + (1.0 / 10.0), texCoordsStatus.v               , -2, i, 1.0));
		statusVertices.push_back(Vertex(1.0, 1.0, texCoordsStatus.u + (1.0 / 10.0), texCoordsStatus.v + (1.0 / 10.0), -2, i, 1.0));
		statusVertices.push_back(Vertex(0.0, 1.0, texCoordsStatus.u               , texCoordsStatus.v + (1.0 / 10.0), -2, i, 1.0));
	}

	// Create Cursor
	cursorVertices.push_back(Vertex(0.0, 0.0, texCoordsStatus.u               , texCoordsStatus.v               , 1, 1, 1.0));
	cursorVertices.push_back(Vertex(0.1, 0.0, texCoordsStatus.u + (1.0 / 10.0), texCoordsStatus.v               , 1, 1, 1.0));
	cursorVertices.push_back(Vertex(0.1, 1.0, texCoordsStatus.u + (1.0 / 10.0), texCoordsStatus.v + (1.0 / 10.0), 1, 1, 1.0));
	cursorVertices.push_back(Vertex(0.0, 1.0, texCoordsStatus.u               , texCoordsStatus.v + (1.0 / 10.0), 1, 1, 1.0));

	area = new TextEditor();
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
	all.insert(all.end(), statusVertices.begin(), statusVertices.end());
	all.insert(all.end(), cursorVertices.begin(), cursorVertices.end());

	openGL.vertexBuffer.SetData(all);
	openGL.indexBuffer.SetData(indices);
}

void Program::UpdateIndices()
{
	int neededIndices = (vertices.size() + marginVertices.size() + commandVertices.size() + statusVertices.size() + cursorVertices.size()) / 4 * 6;

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

void Program::UpdateCursor()
{
	for (int i = 0; i < 4; i++) 
	{
		cursorVertices[i].column = textX + 1;
		cursorVertices[i].row = textY + 1;
	}

	SetData();
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

	StatusResize();

	SetData();

	area->OnResize(width, height);
}

void Program::RenderText(const std::string message)
{
	for (int i = 0; i < statusVertices.size(); i++)
	{
		if (i / 4 >= message.size())
			break;

		TexCoords texCoord = GetCoords(message[i / 4]);

		statusVertices[i].texCoords[0]   = texCoord.u;
		statusVertices[i].texCoords[1]   = texCoord.v;

		statusVertices[++i].texCoords[0] = texCoord.u + (1.0 / 10.0);
		statusVertices[i].texCoords[1]   = texCoord.v;
		
		statusVertices[++i].texCoords[0] = texCoord.u + (1.0 / 10.0);
		statusVertices[i].texCoords[1]   = texCoord.v + (1.0 / 10.0);
		
		statusVertices[++i].texCoords[0] = texCoord.u;
		statusVertices[i].texCoords[1]   = texCoord.v + (1.0 / 10.0);
	}
}

void Program::StatusResize()
{
	float columnAmount = ((1.0 / (textSize * 0.1)) * ((float)width / (float)IDEAL_WIDTH));
	TexCoords texCoords = GetCoords(' ');

	for (int i = 0; i < columnAmount; i++)
	{
		if (i < statusVertices.size() / 4)
			continue;

		statusVertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , -2, i, 1.0));
		statusVertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , -2, i, 1.0));
		statusVertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), -2, i, 1.0));
		statusVertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), -2, i, 1.0));
	}

	UpdateIndices();
	SetData();
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
	commandX = 0;
}

void Program::MoveEndCommand()
{
	commandX = commandText.size();
}

void Program::MoveRightCommand()
{
	if (commandX < commandText.size())
		commandX++;
}

void Program::MoveLeftCommand()
{
	if (commandX > 0)
		commandX--;
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
	// TODO: Fix commandVertices.clear();
	
	// commandText.clear(); 
	// commandX = 0;
	commandSelected = false;
}

void Program::RemoveCharacterFromLeftCommand()
{
	if (commandX == 0)
		return;

	int index = commandX * 4;

	commandVertices.erase(commandVertices.begin() + index, commandVertices.begin() + index + 4);

	for (int i = index; i < commandVertices.size(); i++)
		commandVertices[i].column--;

	indices.resize(indices.size() - 6);

	commandText.erase(commandText.begin() + commandX - 1);

	MoveLeftCommand();

	SetData();
}

void Program::RemoveCharacterFromRightCommand()
{
	// TODO: Fix problems

	// if (commandX = commandText.size())
	// 	return;
	// 
	// int index = commandX * 4 + 4;
	// 
	// commandVertices.erase(commandVertices.begin() + index, commandVertices.begin() + index + 4);
	// 
	// for (int i = index; i < commandVertices.size(); i++)
	// 	commandVertices[i].column--;
	// 
	// indices.resize(indices.size() - 6);
	// 
	// commandText.erase(commandText.begin() + commandX);
	// 
	// SetData();
}

