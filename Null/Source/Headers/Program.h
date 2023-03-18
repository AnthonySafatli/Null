#pragma once

#include "TextArea.h"
#include "GLObejct.h"
#include "GLAbstraction.h"

#include <vector>
#include <string>

#define IDEAL_WIDTH 800
#define IDEAL_HEIGHT 1400

#define KEYCODE_ESCAPE 256

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
	void OpenEditor();
	void OpenEditor(const std::string text, const std::string dir);
	void OpenFile(const std::string dir);
	void OpenJournal(const std::string name);
	void OpenViewer(const std::string str, const std::string pageName);
	void LoadSettings();
	void LoadHelp(const bool commands, const bool shortcuts);

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
	unsigned int rowIndex;
	unsigned int columnIndex;

	/* Command Cursor */
	bool commandSelected;
	unsigned int commandX;

	/* Text Properties */
	bool showCursor;
	unsigned int cursorSpeed;
	int tabAmount;
	float textSize;

	/* Window Properties */
	int width;
	int height;
	const int idealWidth;
	const int idealHeight;
	bool shouldClose;

private:
	void AddCommandSymbol();
};