#pragma once

#include "GLFW/glfw3.h"

#include <vector>
#include <string>
#include <filesystem>

#include "TextArea.h"
#include "GLObejct.h"
#include "GLAbstraction.h"
#include "Command.h"
#include "UndoObject.h"

#define IDEAL_WIDTH 800
#define IDEAL_HEIGHT 1400

class Program
{
public:
	/* Constructor */
	Program(const int width, const int height, const float textSize, const int tabAmount);
	~Program();

	/* Window */
	GLFWwindow* window;

	/* OpenGL */
	GLObject openGL;
	void GLInit();
	std::vector<Vertex> GetVertices();

	/* Callback */
	void ProcessKey(int key, int action, int mods);
	void ProcessChar(unsigned int codepoint);
	void OnResize(int width, int height);
	void OnScroll(double xOffset, double yOffset);

	/* Update */
	void Update(const double deltaTime);

	/* Command Callback */
	void ProcessKeyCommand(int key, int action, int mods);
	void ProcessCharCommand(unsigned int codepoint);

	/* Command Methods */
	void AddCharacterCommand(const char ch);
	void MoveHomeCommand();
	void MoveEndCommand();
	void MoveRightCommand();
	void MoveLeftCommand();
	void ToggleAutoComplete();
	void ScrollUpAutoComplete();
	void ScrollDownAutoComplete();
	void EnterCommand();
	void RemoveCharacterFromLeftCommand();
	void RemoveCharacterFromRightCommand();

	/* Status Methods */
	void RenderStatus(const std::string message);
	void StatusResize();

	/* Vertex Data */
	std::vector<Vertex> vertices;
	std::vector<Vertex> commandVertices;
	std::vector<Vertex> statusVertices;
	std::vector<Vertex> marginVertices;
	std::vector<Vertex> cursorVertices;
	std::vector<unsigned int> indices;
	void UpdateIndices();

	/* TextArea Methods */
	void OpenEditor();                                                                     // Opens new Text Editor
	void OpenEditor(const std::string text, const std::string path, bool isNote);          // Opens Text Editor with text
	void OpenFile(const std::string path);                                                 // Opens File into Text Editor
	void OpenTextViewer(const std::string str, const std::string pageName, AreaType type); // Opens Text Viewer with text
	void OpenNoteViewer();                                                                 // Opens Note File View at root
	void OpenNoteViewer(std::vector<std::string> folders);                                 // Opens Note File View at path
	void OpenNote(std::filesystem::path notePath, std::string noteName);                   // Opens Note in Text Editor
	void LoadSettings();                                                                   // Opens Text Viewer with Settings text

	/* Cursor Methods */
	void ShowCursor();
	void HideCursor();

	/* Text Data */
	std::string commandText;
	TextArea *area;
	std::string statusText;
	std::string statusTextRight;

	/* Text Cursor */
	unsigned int textX;
	unsigned int textY;
	double rowIndex;
	double columnIndex;

	/* Command Cursor */
	bool commandSelected;
	unsigned int commandX;

	/* Text Properties */
	bool showCursor;
	int tabAmount;

	/* Settings */
	float textSize;
	Colour background;
	Colour foreground;
	unsigned int cursorSpeed;

	/* Window Properties */
	int width;
	int height;
	const int idealWidth;
	const int idealHeight;
	bool shouldClose;

private:
	void AddCommandSymbol();
};