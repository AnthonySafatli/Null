// VERTEX
#version 330 core

const uint INVISIBLE = 0;
const uint NORMAL = 1;
const uint COMMAND_LINE = 2;
const uint STATUS_TEXT = 3;

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
in layout(location = 4) uint type;

out vec2 vTexCoords;
out float vRow;
flat out uint vType;

void main() {
	vTexCoords = texCoords;
	vRow = row;
	vType = type;

	// Remove out of bounds letters
	if ((row < rowIndex || row - rowIndex > maxHeight) && type != COMMAND_LINE)
		vType = INVISIBLE;

	if ((column < columnIndex || column - rowIndex > maxWidth) && type != COMMAND_LINE)
		vType = INVISIBLE;

	// Get actual row column
	float actualRow = row + 3;
	float actualColumn = leftMargin;

	if (type == STATUS_TEXT)
		actualRow = ((1.0 / size) * (1.0 / idealRatio.y)) - 1;

	// Calculate x y position
	vec2 position = reletivePosition * size;
	position.y += actualRow * size;
	position.x += actualColumn * size; 

	// Const pixel size conversions
	vec2 constPixel = idealRatio * position;

	// Screen space conversions
	vec2 screenSpace = constPixel * 2 - 1;
	screenSpace.y *= -1;

	// Set position
	gl_Position = vec4(screenSpace, 0.0, 1.0);
}