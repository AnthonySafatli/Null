#include "Headers/CursorController.h"
#include "Headers/Contents.h"

extern Contents appContents;

CursorController::CursorController(int x, int y) : x(x), y(y), sceneStartIndex(0)
{
}

void CursorController::Move(const Direction dir)
{
	// TODO: Proper bounds checking

	switch (dir)
	{
	case UP:

		if (y > 0)
			y--;

		break;

	case DOWN:

		if (y >= appContents.currentScene.text.size())
			y++;

		break;

	case LEFT:
		x--;
		break;

	case HOME:

		// TODO: Implement 'HOME'

		break;

	case END:

		// TODO: Implement 'END'

		break;

	case RIGHT:

		TextRow currentRow = appContents.currentScene.text[appContents.cursor.y];
		if (x < currentRow.text.size())
			x++;

		break;
	}
}
