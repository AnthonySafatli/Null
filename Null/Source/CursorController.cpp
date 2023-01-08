#include "Headers/CursorController.h"
#include "Headers/Contents.h"

extern Contents appContents;

CursorController::CursorController(int x, int y) : x(x), y(y), sceneStartRowIndex(0), sceneStartColumnIndex(0), sceneLeftBarrier(3)
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

		if (y < appContents.currentScene.rows.size())
			y++;

		break;

	case LEFT:

		if (x > sceneLeftBarrier)
			x--;

		break;

	case HOME:

		// TODO: Implement 'HOME'

		break;

	case END:

		// TODO: Implement 'END'

		break;

	case RIGHT:

		TextRow currentRow = appContents.currentScene.rows[appContents.cursor.y];
		if (x < currentRow.text.size())
			x++;

		break;
	}
}
