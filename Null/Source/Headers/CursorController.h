#pragma once

#include "Scene.h"

enum Direction { UP, DOWN, RIGHT, LEFT, HOME, END };

class CursorController
{
private: 

	unsigned int screenX;
	unsigned int screenY;

public:
	CursorController(int x, int y);

	void Move(const Direction dir);

	unsigned int textX;
	unsigned int textY;
	unsigned int sceneStartRowIndex;    // the index of the first row in the Scene
	unsigned int sceneStartColumnIndex; // the index of the first column in the Scene
	unsigned int sceneLeftBarrier;      // amount of columns that are not accessible to cursor to left
};