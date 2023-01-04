#include "Headers/CursorController.h"

CursorController::CursorController(int x, int y) : screenX(x), screenY(y), sceneStartIndex(0)
{

}

void CursorController::Move(const Direction dir)
{
	// TODO: Bounds checking

	switch (dir)
	{
	case UP:
		screenY--;
		break;
	case DOWN:
		screenY++;
		break;
	case RIGHT:
		screenX++;
		break;
	case LEFT:
		screenX--;
		break;
	default:
		break;
	}
}
