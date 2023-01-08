#include "Headers/CursorController.h"
#include "Headers/Contents.h"

extern Contents appContents;

CursorController::CursorController(int x, int y) 
	: textX(0), textY(0), screenX(x), screenY(y), sceneStartRowIndex(0), sceneStartColumnIndex(0), sceneLeftBarrier(3)
{
}

void CursorController::Move(const Direction dir)
{
	// TODO: Proper bounds checking

	switch (dir)
	{
	case HOME:

		// TODO: Implement 'HOME'

		break;

	case END:

		// TODO: Implement 'END'

		break;
	
	case UP:

		if (textY > 0)
			textY--;

		break;

	case DOWN:

		if (textY < appContents.currentScene.rows.size())
			textY++;

		break;

	case LEFT:

		if (textX > sceneLeftBarrier)
			textX--;

		break;

	case RIGHT:

		TextRow currentRow = appContents.currentScene.rows[textY];
		if (textX < currentRow.text.size())
			textX++;

		break;
	}
}
