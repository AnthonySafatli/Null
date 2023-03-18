// #include "Headers/CursorController.h"
// #include "Headers/NullEditor.h"

// #include <iostream>

// extern NullEditor appContents;

/*

CursorController::CursorController(int x, int y)
	: textX(0), textY(0), screenX(x), screenY(y), commandX(3), isOnCommand(false),
	  sceneStartRowIndex(0), sceneStartColumnIndex(0), sceneLeftBarrier(3)
{
}

void CursorController::Move(const Direction dir)
{
	switch (dir)
	{
	case HOME:
		Home();
		break;

	case END:
		End();
		break;
	
	case UP:
		Up();
		break;

	case DOWN:
		Down();
		break;

	case LEFT:
		Left();
		break;

	case RIGHT:
		Right();
		break;
	}
}

void CursorController::Home()
{
	if (isOnCommand)
	{
		commandX = 0;

		//for (int i = 0; appContents.command.row.text[i] == ' '; i++)
		for (int i = 0; 'c' == ' '; i++)
			commandX++;
		
		return;
	}

	textX = sceneLeftBarrier;

	// for (int i = sceneLeftBarrier; appContents.currentScene.rows[textY].text[i] == ' '; i++)
	for (int i = sceneLeftBarrier; 'c' == ' '; i++)
		textX++;
}

void CursorController::End()
{
	if (isOnCommand)
		//commandX = appContents.command.row.text.size();
		commandX = 0;
	else
		//textX = appContents.currentScene.rows[textY].text.size();
		textX = 0;
}

void CursorController::Up()
{
	if (isOnCommand)
		return;

	else if (screenY > 0)
	{
		textY--;
		screenY--;
	}
	else if (textY > 0)
	{
		textY--;
		//appContents.SetRowIndex(--appContents.rowIndex);
	}
}

void CursorController::Down()
{
	if (isOnCommand)
		return;

	//if (textY >= appContents.currentScene.rows.size() - 1)
	if (textY >= 0 - 1)
		return;
	
	//if (screenY + 5.5 < (1.0 / (0.1 * appContents.textSize)) * ((float)appContents.height / (float)appContents.idealHeight))
	if (screenY + 5.5 < (1.0 / (0.1 * 0)) * ((float)0 / (float)0))
	{
		screenY++;
		textY++;
	}
	else
	{
		textY++;
		//appContents.SetRowIndex(++appContents.rowIndex);
	}
}

void CursorController::Left()
{
	if (isOnCommand && commandX > 3)
	{
		commandX--;
		return;
	}

	if (textX > sceneLeftBarrier)
		textX--;
}

void CursorController::Right()
{
	//if (isOnCommand && commandX < appContents.command.row.text.size() + 3)
	if (isOnCommand && commandX < 0 + 3)
	{
		commandX++;
		return;
	}

	//if (textX < appContents.currentScene.rows[textY].text.size())
	if (textX < 0)
		textX++;
}

*/