// VERTEX
#version 330 core

const float COMMAND_LINE_ROW = -1.0;
const float STATUS_BAR_ROW   = -2.0;

uniform vec2 idealRatio; // idealSize / currentSize
// uniform float height;
uniform float size;
uniform int sceneRowIndex;
uniform int sceneColumnIndex;

in layout(location = 0) vec2 reletivePosition;
in layout(location = 1) vec2 texCoords;
in layout(location = 2) float row;
in layout(location = 3) float column;
in layout(location = 4) uint highlight;

out vec2 vTexCoords;
out float vRow;
flat out uint vHighlight;

void main() {
	vTexCoords = texCoords;
	vRow = row;
	vHighlight = highlight;

	float actualRow = 0;
	float actualColumn = column - sceneColumnIndex;

	if (row != COMMAND_LINE_ROW && row != STATUS_BAR_ROW) 
	{
		actualRow = row - sceneRowIndex;
		actualRow += 2.0;
	}

	if (row == STATUS_BAR_ROW)
		actualRow = ((1.0 / size) * (1.0 / idealRatio.y)) - 1;
	else
		actualRow += 0.5;

	// calculate position
	vec2 position = reletivePosition * size;
	position.y += actualRow * size;
	position.x += actualColumn * size; 

	// const pixel size conversions
	vec2 constPixel = idealRatio * position;

	// screen space conversions
	vec2 screenSpace = constPixel * 2 - 1;
	screenSpace.y *= -1;

	gl_Position = vec4(screenSpace, 0.0, 1.0);
}