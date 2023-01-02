#include "Headers/Contents.h"

/* ====== Contents ====== */

Contents::Contents() : textSize(0.1), cursor(0, 0)
{
	// Only run once at beginning of program
}

void Contents::AddCharacter(char ch)
{
	// use cursor controller to find which row cursor is on
	// use text.size() for all rows above cursor to see how many characters to skip
	// use column in cursor controller to see how many more characters
	// multiply by 4 to get how many vertices to skip
	// add vertices at that index
}

void Contents::RemoveCharacter()
{
	// same as adding but not
}
