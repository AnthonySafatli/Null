#include "Headers/CursorController.h"
#include "Headers/Contents.h"

CursorController::CursorController(int x, int y) : x(x), y(y), sceneStartIndex(0)
{

}

void CursorController::Move(const Direction dir)
{
	// TODO: Bounds checking

	switch (dir)
	{
	case UP:

		if (y > 0)
			y--;

		break;

	case DOWN:

		if (y >= Contents::currentScene.text.size())
			y++;

		break;

	case LEFT:

		if (x > 0)
			x--;

		break;

	case RIGHT:

		TextRow currentRow = Contents::currentScene.text[Contents::cursor.y];
		if (x < currentRow.text.size())
			x++;

		break;
	}
}
