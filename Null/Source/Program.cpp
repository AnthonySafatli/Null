#include "Headers/Program.h"

#include "GLFW/glfw3.h"

#include <filesystem>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Headers/TextArea.h"
#include "Headers/TextEditor.h"
#include "Headers/TextViewer.h"
#include "Headers/NoteViewer.h"
#include "Headers/GLAbstraction.h"
#include "Headers/Uniforms.h"
#include "Headers/Character.h"
#include "Headers/Command.h"
#include "Headers/Misc.h"
#include "Headers/Shortcut.h"
#include "Headers/UndoObject.h"

Program::Program(const int width, const int height, const float textSize, const int tabAmount)
	: idealWidth(IDEAL_WIDTH), idealHeight(IDEAL_HEIGHT), 
	  height(height), width(width), textSize(textSize), tabAmount(tabAmount), showCursor(false), cursorSpeed(50),
	  rowIndex(0), columnIndex(0), textX(0), textY(0), commandX(0), commandSelected(false), shouldClose(false),
	  background(0.03, 0.05, 0.09, 0.85), foreground(1, 1, 1), window(0), showDate(true)
{
	// Add > to Command Line
	AddCommandSymbol();

	// Create Status Bar
	float columnAmount = ((1.0 / (textSize * 0.001)) * ((float)width / (float)IDEAL_WIDTH));
	TexCoords texCoordsStatus = GetCoords(' ');

	for (int i = 0; i < columnAmount; i++)
	{
		statusVertices.push_back(Vertex(0.0, 0.0, texCoordsStatus.u               , texCoordsStatus.v               , -2, i, STATUS));
		statusVertices.push_back(Vertex(1.0, 0.0, texCoordsStatus.u + (1.0 / 10.0), texCoordsStatus.v               , -2, i, STATUS));
		statusVertices.push_back(Vertex(1.0, 1.0, texCoordsStatus.u + (1.0 / 10.0), texCoordsStatus.v + (1.0 / 10.0), -2, i, STATUS));
		statusVertices.push_back(Vertex(0.0, 1.0, texCoordsStatus.u               , texCoordsStatus.v + (1.0 / 10.0), -2, i, STATUS));
	}

	// Create Cursor
	cursorVertices.push_back(Vertex(0.0, 0.0, texCoordsStatus.u               , texCoordsStatus.v               , 1, 1, CURSOR));
	cursorVertices.push_back(Vertex(0.1, 0.0, texCoordsStatus.u + (1.0 / 10.0), texCoordsStatus.v               , 1, 1, CURSOR));
	cursorVertices.push_back(Vertex(0.1, 1.0, texCoordsStatus.u + (1.0 / 10.0), texCoordsStatus.v + (1.0 / 10.0), 1, 1, CURSOR));
	cursorVertices.push_back(Vertex(0.0, 1.0, texCoordsStatus.u               , texCoordsStatus.v + (1.0 / 10.0), 1, 1, CURSOR));

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

std::vector<Vertex> Program::GetVertices()
{
	std::vector<Vertex> allVertices = cursorVertices;
	allVertices.insert(allVertices.end(), commandVertices.begin(), commandVertices.end());
	allVertices.insert(allVertices.end(), statusVertices.begin(), statusVertices.end());
	allVertices.insert(allVertices.end(), marginVertices.begin(), marginVertices.end());
	allVertices.insert(allVertices.end(), vertices.begin(), vertices.end());

	return allVertices;
}

void Program::UpdateIndices()
{
	int neededIndices = (vertices.size() + marginVertices.size() + commandVertices.size() + statusVertices.size() + cursorVertices.size()) / 4 * 6;

	int indexNum = neededIndices - indices.size();
	int count = indexNum / 6;

	if (count < 0)
		indices.resize(indices.size() + indexNum);

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

void Program::AddCommandSymbol()
{
	TexCoords texCoordsCommand = GetCoords('>');

	commandVertices.push_back(Vertex(0.0, 0.0, texCoordsCommand.u               , texCoordsCommand.v               , -1, 2, COMMAND));
	commandVertices.push_back(Vertex(1.0, 0.0, texCoordsCommand.u + (1.0 / 10.0), texCoordsCommand.v               , -1, 2, COMMAND));
	commandVertices.push_back(Vertex(1.0, 1.0, texCoordsCommand.u + (1.0 / 10.0), texCoordsCommand.v + (1.0 / 10.0), -1, 2, COMMAND));
	commandVertices.push_back(Vertex(0.0, 1.0, texCoordsCommand.u               , texCoordsCommand.v + (1.0 / 10.0), -1, 2, COMMAND));
}

void Program::ProcessKey(int key, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		commandSelected = !commandSelected;
		for (int i = 0; i < 4; i++) cursorVertices[i].type = commandSelected ? CURSOR_COMMAND : (showCursor ? CURSOR : CURSOR_INVISIBLE);
		return;
	}

	Shortcut::UniversalShortcuts(key, action, mods);

	if (commandSelected)
	{
		ShowCursor();

		Shortcut::CommandShortcuts(key, action, mods);

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

	UpdateMaxHeightWidth();
	StatusResize();
	
	area->OnResize(width, height);
}

void Program::OnScroll(double xOffset, double yOffset)
{
	rowIndex -= yOffset;
	columnIndex -= xOffset;

	if (rowIndex < 0)
		rowIndex = 0;
	else if (rowIndex >= area->rows.size())
		rowIndex = area->rows.size() - 1;

	int longestRow = area->LongestRowSize();

	if (longestRow == 0)
		columnIndex = 0;
	else if (columnIndex < 0)
		columnIndex = 0;
	else if (columnIndex >= longestRow)
		columnIndex = area->LongestRowSize() - 1;

	UpdateUniform1i(openGL.u_rowIndex.location, (int)rowIndex);
	UpdateUniform1i(openGL.u_columnIndex.location, (int)columnIndex);
}

void Program::Update(const double deltaTime)
{
	/* Cursor Animations */
	float targetRow = commandSelected ? (float)(((int)rowIndex) - 1) : textY + 1;
	float targetColumn = commandSelected ? (float)(commandX + 4 - area->leftMargin + ((int)columnIndex)) : textX + 1;

	float deltaRow = deltaTime * (targetRow - cursorVertices[0].row) * (float)cursorSpeed;
	float deltaColumn = deltaTime * (targetColumn - cursorVertices[0].column) * (float)cursorSpeed;

	if (std::abs(deltaColumn) < 0.0001) deltaColumn = 0;
	
	bool rowSet = false;
	bool colSet = false;

	// Overshooting logic
	if (deltaRow > 0 && cursorVertices[0].row + deltaRow > (commandSelected ? -1.0f : (float)(textY + 1)))
	{
		for (int i = 0; i < 4; i++) cursorVertices[i].row = targetRow; 
		rowSet = true;
	}
	else if (deltaRow < 0 && cursorVertices[0].row + deltaRow < (commandSelected ? -1.0f : (float)(textY + 1)))
	{
		for (int i = 0; i < 4; i++) cursorVertices[i].row = targetRow;
		rowSet = true;
	}

	if (deltaColumn > 0 && cursorVertices[0].column + deltaColumn > (commandSelected ? (float)(commandX + 1) : (float)(textX + 1)))
	{
		for (int i = 0; i < 4; i++) cursorVertices[i].column = targetColumn;
		colSet = true;
	}
	else if (deltaColumn < 0 && cursorVertices[0].column + deltaColumn < (commandSelected ? (float)(commandX + 1) : (float)(textX + 1)))
	{
		for (int i = 0; i < 4; i++) cursorVertices[i].column = targetColumn;
		colSet = true;
	}

	for (int i = 0; i < 4; i++)
	{
		if (!rowSet)
			cursorVertices[i].row += deltaRow;

		if (!colSet)
			cursorVertices[i].column += deltaColumn;
	}

	UpdateIndices();
}

/* Status Methods */

void Program::RenderStatus(const std::string message)
{
	statusText = message;

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

	TexCoords spaceCoords = GetCoords(' ');
	for (int i = message.size() * 4; i < statusVertices.size(); i++)
	{
		statusVertices[i].texCoords[0] = spaceCoords.u;
		statusVertices[i].texCoords[1] = spaceCoords.v;

		statusVertices[++i].texCoords[0] = spaceCoords.u + (1.0 / 10.0);
		statusVertices[i].texCoords[1] = spaceCoords.v;

		statusVertices[++i].texCoords[0] = spaceCoords.u + (1.0 / 10.0);
		statusVertices[i].texCoords[1] = spaceCoords.v + (1.0 / 10.0);

		statusVertices[++i].texCoords[0] = spaceCoords.u;
		statusVertices[i].texCoords[1] = spaceCoords.v + (1.0 / 10.0);
	}
}

void Program::StatusResize()
{
	float columnAmount = ((1.0 / (textSize * 0.001)) * ((float)width / (float)IDEAL_WIDTH));
	TexCoords texCoords = GetCoords(' ');

	for (int i = 0; i < columnAmount; i++)
	{
		if (i < statusVertices.size() / 4)
			continue;

		statusVertices.push_back(Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , -2, i, STATUS));
		statusVertices.push_back(Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , -2, i, STATUS));
		statusVertices.push_back(Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), -2, i, STATUS));
		statusVertices.push_back(Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), -2, i, STATUS));
	}
}

/* Command Line Methods */

void Program::ProcessKeyCommand(int key, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	Shortcut::CommandShortcuts(key, action, mods);

	switch (key)
	{
	case GLFW_KEY_HOME:
		MoveHomeCommand();
		break;
	case GLFW_KEY_KP_7:
		// TODO: Numpad stuff here too
		MoveHomeCommand();
		break;
	case GLFW_KEY_END:
		MoveEndCommand();
		break;
	case GLFW_KEY_KP_1:
		// TODO: Numpad stuff here too
		MoveEndCommand();
		break;
	case GLFW_KEY_RIGHT:
		MoveRightCommand();
		break;
	case GLFW_KEY_LEFT:
		MoveLeftCommand();
		break;

	case GLFW_KEY_TAB:
		ToggleAutoComplete();
		break;
	case GLFW_KEY_UP:
		ScrollUpAutoComplete();
		break;
	case GLFW_KEY_DOWN:
		ScrollDownAutoComplete();
		break;

	case GLFW_KEY_ENTER:
		EnterCommand();
		break;
		
	case GLFW_KEY_BACKSPACE:
		RemoveCharacterFromLeftCommand();
		break;
	case GLFW_KEY_DELETE:
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

	commandVertices.insert(commandVertices.begin() + offset++, Vertex(0.0, 0.0, texCoords.u               , texCoords.v               , -1, commandX + 4, COMMAND));
	commandVertices.insert(commandVertices.begin() + offset++, Vertex(1.0, 0.0, texCoords.u + (1.0 / 10.0), texCoords.v               , -1, commandX + 4, COMMAND));
	commandVertices.insert(commandVertices.begin() + offset++, Vertex(1.0, 1.0, texCoords.u + (1.0 / 10.0), texCoords.v + (1.0 / 10.0), -1, commandX + 4, COMMAND));
	commandVertices.insert(commandVertices.begin() + offset++, Vertex(0.0, 1.0, texCoords.u               , texCoords.v + (1.0 / 10.0), -1, commandX + 4, COMMAND));

	for (int i = offset; i < commandVertices.size(); i++)
		commandVertices[i].column++;

	commandText.insert(commandText.begin() + commandX, ch);

	MoveRightCommand();
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
	Command::Execute(commandText);

	commandVertices.clear();
	AddCommandSymbol();
	commandText.clear(); 
	commandX = 0;
	commandSelected = false;

	if (!showCursor)
		HideCursor();
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
}

void Program::RemoveCharacterFromRightCommand()
{
	if (commandX == commandText.size())
		return;
	
	int index = commandX * 4 + 4;
	
	commandVertices.erase(commandVertices.begin() + index, commandVertices.begin() + index + 4);
	
	for (int i = index; i < commandVertices.size(); i++)
		commandVertices[i].column--;
	
	commandText.erase(commandText.begin() + commandX);
}

/* TextArea Methods */

void Program::OpenEditor()
{
	vertices.clear();
	marginVertices.clear();

	delete area;

	area = new TextEditor();
	glfwSetWindowTitle(window, "New File - Null");
}

void Program::OpenEditor(const std::string text, const std::string path, bool isNote)
{
	vertices.clear();
	marginVertices.clear();

	delete area;

	std::filesystem::path pathObj(path);

	std::string fileName = pathObj.filename().string();
	if (isNote)
		fileName = fileName.substr(3, fileName.size() - 9);

	area = new TextEditor(text, path, fileName, isNote);
	glfwSetWindowTitle(window, (fileName + " - Null").c_str());
}

void Program::OpenFile(const std::string path)
{
	if (!std::filesystem::is_regular_file(std::filesystem::path(path)))
	{
		RenderStatus("Error occurred while opening " + path);
		return;
	}

	std::ifstream file(path);
	if (!file.is_open())
	{
		RenderStatus("Error occurred while opening " + path);
		return;
	}

	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	OpenEditor(str, path, false);
}

void Program::OpenTextViewer(const std::string str, const std::string pageName)
{
	vertices.clear();
	marginVertices.clear();

	delete area;

	area = new TextViewer(str, pageName);
	glfwSetWindowTitle(window, (pageName + " - Null").c_str());
}

void Program::OpenNoteViewer()
{
	vertices.clear();
	marginVertices.clear();

	std::filesystem::path documents = NoteViewer::GetDocumentsFolder();

	if (documents.empty())
	{
		RenderStatus("Notebook is unavailable at the moment");
		return;
	}

	delete area;

#if _DEBUG
	std::cout << documents.string() << std::endl;
#endif

	area = new NoteViewer(documents);
	glfwSetWindowTitle(window, "Notes - Null");
}

void Program::OpenNoteViewer(std::vector<std::string> folders)
{
	vertices.clear();
	marginVertices.clear();

	std::filesystem::path documents = NoteViewer::GetDocumentsFolder();
	if (documents.empty())
	{
		RenderStatus("Notebook is unavailable at the moment");
		return;
	}

	std::filesystem::path noteFolder(documents / "NullNotes");
	for (std::string folder : folders) noteFolder = noteFolder / folder;
	if (!std::filesystem::is_directory(noteFolder))
	{
		RenderStatus("Note folder not found");
		return;
	}

	delete area;

#if _DEBUG
	std::cout << documents.string() << std::endl;
#endif

	area = new NoteViewer(documents, folders);
	glfwSetWindowTitle(window, "Notes - Null");
}

void Program::OpenNote(std::filesystem::path notePath, std::string noteName)
{
	if (!std::filesystem::is_regular_file(notePath))
	{
		RenderStatus("Error occurred while opening " + noteName);
		return;
	}

	std::ifstream file(notePath);
	if (!file.is_open())
	{
		RenderStatus("Error occurred while opening " + noteName);
		return;
	}

	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	OpenEditor(str, notePath.string(), true);
}

void Program::LoadSettings()
{
	const std::string settings = "Settings:"
		"\n\n"
		"Size: " + FloatToString(textSize) +
		"\n\n"
		"Background: " + FloatToString(background.r) + " " + FloatToString(background.g) + " " + FloatToString(background.b) + " " + FloatToString(background.a) +
		"\n\n"
		"Foreground: " + FloatToString(background.r) + " " + FloatToString(background.g) + " " + FloatToString(background.b) + " " + FloatToString(background.a) +
		"\n\n"
		"Speed: " + FloatToString(cursorSpeed);

	OpenTextViewer(settings, "Settings");
}

void Program::LoadHelp(const bool commands, const bool shortcuts)
{
	// TODO: Write out help string
	const std::string help = 
		"Help:\n"
		"Commands:\n\n\n"
		"Shortcuts:\n";

	// TODO: Make help command and shortcut strings

	OpenTextViewer(help, "Help");
}

/* Cursor Methods */

void Program::ShowCursor()
{
	for (int i = 0; i < 4; i++) cursorVertices[i].type = commandSelected ? CURSOR_COMMAND : CURSOR;
}

void Program::HideCursor()
{
	for (int i = 0; i < 4; i++) cursorVertices[i].type = CURSOR_INVISIBLE;
}
