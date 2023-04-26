// VERTEX
#version 330 core

#define INVISIBLE        0.0 // Invisible text
#define NORMAL           1.0 // Normal text
#define CURSOR           2.0 // Normal cursor
#define CURSOR_COMMAND   3.0 // Cursor in command-line
#define CURSOR_INVISIBLE 4.0 // Invisible cursor
#define STATUS           5.0 // Status bar text
#define COMMAND          6.0 // Command line text
#define MARGIN           7.0 // Margin text
#define MARGIN_SELECTED  8.0 // Margin text selected

uniform vec2 idealRatio; // idealSize / currentSize
uniform float size;
uniform int rowIndex;
uniform int columnIndex;
uniform int leftMargin;
uniform int maxHeight;
uniform int maxWidth;

in layout(location = 0) vec2 reletivePosition;
in layout(location = 1) vec2 texCoords;
in layout(location = 2) float row;
in layout(location = 3) float column;
in layout(location = 4) float type;

out vec2 vTexCoords;
out float vRow;
out float vType;

void main() {
	vTexCoords = texCoords;
	vRow = row;
	vType = type;

	if (type == INVISIBLE || type == CURSOR_INVISIBLE)
		return;

	// Remove out of bounds letters
	if (type == NORMAL || type == MARGIN || type == MARGIN_SELECTED)
	{
		if (row - rowIndex <= 0)
			vType = INVISIBLE;
		if (row - rowIndex > maxHeight)
			vType = INVISIBLE;
	}
	if (type == NORMAL)	
	{
		if (column - columnIndex <= 0)
			vType = INVISIBLE;
	}

	// Remove out of bounds cursor
	if (type == CURSOR)
	{
		if (row - rowIndex <= 0)
			vType = CURSOR_INVISIBLE;
		if (row - rowIndex > maxHeight)
			vType = CURSOR_INVISIBLE;
		if (column - columnIndex < 0.01)
			vType = CURSOR_INVISIBLE;
	}

	// Calculate screen row and column
	float actualRow = row - rowIndex + 2;
	float actualColumn = column - columnIndex + leftMargin;

	// Set type row and column
	if (type == STATUS)
	{
		// Fixed to bottom of viewport
		actualRow = ((1.0 / size) * (1.0 / idealRatio.y)) - 1;
		actualColumn = column;
	}
	if (type == COMMAND)
	{
		// Fixed to top of viewport
		actualRow = 1;
		actualColumn = column;
	}
	if (type == MARGIN || type == MARGIN_SELECTED)
	{
		// Fixed to left of viewport
		actualColumn = column + leftMargin;
	}

	// Calculate viewport position
	vec2 position = reletivePosition * size;
	position.y += actualRow * size;
	position.x += actualColumn * size; 

	// Constant pixel size conversions
	vec2 constPixel = idealRatio * position;

	// Screen space conversions
	vec2 screenSpace = constPixel * 2 - 1;
	screenSpace.y *= -1;

	gl_Position = vec4(screenSpace, 0.0, 1.0);
}