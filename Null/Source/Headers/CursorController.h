#pragma once

#include "Scene.h"

enum Direction { UP, DOWN, RIGHT, LEFT, HOME, END };

class CursorController
{
private: 

	int screenX; // Location on screen
	int screenY; // Location on screen

public:
	CursorController(int x, int y);

	void Move(const Direction dir);

	int textX;   // Location in text
	int textY;   // Location in text
	unsigned int sceneStartRowIndex;    // the index of the first row in the Scene
	unsigned int sceneStartColumnIndex; // the index of the first column in the Scene
	unsigned int sceneLeftBarrier;      // amount of columns that are not accessible to cursor to left
};