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
uniform int sceneRowIndex;
uniform int sceneColumnIndex;
uniform int leftMargin;

// uniform int maxHeight;
// uniform int maxWidth;

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
	if (type == NORMAL)
	{
		if (row - leftMargin < 0)
			vType = INVISIBLE;
//		if (row - leftMargin > maxHeight)
//			vType = INVISIBLE;

		// TODO: Same for width
	}

	// Calculate screen row and column
	float actualRow = row - sceneRowIndex + 2;
	float actualColumn = column - sceneColumnIndex + leftMargin;

	// Set type row and column
	if (type == STATUS)
	{
		actualRow = ((1.0 / size) * (1.0 / idealRatio.y)) - 1;
		actualColumn = column;
	}
	if (type == COMMAND)
	{
		actualRow = 1;
		actualColumn = column + 3;
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