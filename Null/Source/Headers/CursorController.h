#pragma once

#include "Scene.h"

enum Direction { UP, DOWN, RIGHT, LEFT, HOME, END };

class CursorController
{
private: 
	void Home();
	void End();
	void Up();
	void Down();
	void Left();
	void Right();

public:
	CursorController(int x, int y);

	void Move(const Direction dir);
	
	bool isOnCommand;      // Checks if is typing in Command Row
	unsigned int commandX; // Cursor position in Command Row

	unsigned int textX;   // Location in text
	unsigned int textY;   // Location in text

	unsigned int screenX; // Location on screen
	unsigned int screenY; // Location on screen

	unsigned int sceneStartRowIndex;    // the index of the first row in the Scene
	unsigned int sceneStartColumnIndex; // the index of the first column in the Scene
	unsigned int sceneLeftBarrier;      // amount of columns that are not accessible to cursor to left
};