#pragma once

#include "Scene.h"

enum Direction { UP, DOWN, RIGHT, LEFT, HOME, END };

class CursorController
{
public:
	CursorController(int x, int y);

	void Move(const Direction dir);

	unsigned int x;
	unsigned int y;
	unsigned int sceneStartIndex; // the index of the first row in the Scene
};