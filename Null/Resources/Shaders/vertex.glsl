// VERTEX
#version 330 core

#define INVISIBLE 0.0
#define NORMAL    1.0
#define CURSOR    2.0
#define STATUS    3.0
#define COMMAND   4.0
#define MARGIN    5.0

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

	// Remove out of bounds letters
	if (type == NORMAL || type == MARGIN)
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
		actualColumn = column + 3;
	}
	if (type == MARGIN)
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