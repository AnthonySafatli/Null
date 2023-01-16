#include "Headers/CursorController.h"
#include "Headers/NullEditor.h"

#include <iostream>

extern NullEditor appContents;

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
		{
			textY--;
			screenY--;
		}

		break;

	case DOWN:

		if (isOnCommand)
			break;

		if (textY < appContents.currentScene.rows.size() - 1)
		{
			if (screenY + 4.5 < (1.0 / (0.1 * appContents.textSize)) * ((float) appContents.height / (float) appContents.idealHeight))
			{
				screenY++;
				textY++;
			}
			else
			{
				textY++;
				appContents.SetRowIndex(++appContents.rowIndex);
			}
		}

		break;

	case LEFT:

		if (isOnCommand && commandX > 3)
		{
			commandX--;
			break;
		}

		if (textX > sceneLeftBarrier)
			textX--;

		break;

	case RIGHT:

		if (isOnCommand && commandX < appContents.command.text.vertices.size() / 4 + 2)
		{
			commandX++;
			break;
		}

		if (textX < appContents.currentScene.rows[textY].text.size())
			textX++;

		break;
	}
}
