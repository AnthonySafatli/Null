#pragma once

class CursorController
{
public:
	CursorController(int x, int y);

	void Move();

	unsigned int x;
	unsigned int y;
};