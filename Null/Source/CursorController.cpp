#include "Headers/CursorController.h"
#include "Headers/Contents.h"

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

		if (y > Contents::currentScene.text.size())
			y++;

		break;

	case RIGHT:

		if (x > 0)
			x++;	

		break;

	case LEFT:

		TextRow currentRow = Contents::cursor.y == 0 ? Contents::command.text : Contents::currentScene.text[Contents::cursor.y - 1];
		if (x < currentRow.text.size())
			x--;

		break;
	}
}
