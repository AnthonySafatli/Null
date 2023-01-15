#include "Headers/CursorController.h"
#include "Headers/Contents.h"

#include <iostream>

extern Contents appContents;

CursorController::CursorController(int x, int y)
	: textX(0), textY(0), screenX(x), screenY(y), commandX(3), isOnCommand(false),
	  sceneStartRowIndex(0), sceneStartColumnIndex(0), sceneLeftBarrier(3)
{
}

void CursorController::Move(const Direction dir)
{
	// TODO: Proper bounds checking
	// TODO: Add Command bounds checking

	switch (dir)
	{
	case HOME:

		textX = sceneLeftBarrier;

		for (int i = sceneLeftBarrier; appContents.currentScene.rows[textY].text[i] == ' '; i++)
			textX++;

		break;

	case END:

		textX = appContents.currentScene.rows[textY].text.size();

		break;
	
	case UP:

		if (isOnCommand)
			break;

		if (textY > 0)
			textY--;

		break;

	case DOWN:

		if (isOnCommand)
			break;

		if (textY < appContents.currentScene.rows.size() - 1)
			textY++;

		break;

	case LEFT:

		if (isOnCommand)
		{
			commandX--;
			break;
		}

		if (textX > sceneLeftBarrier)
			textX--;

		break;

	case RIGHT:

		if (isOnCommand)
		{
			commandX++;
			break;
		}

		TextRow currentRow = appContents.currentScene.rows[textY];
		if (textX < currentRow.text.size())
			textX++;

		break;
	}
}
