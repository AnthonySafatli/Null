// VERTEX
#version 330 core

uniform vec2 idealRatio; // idealSize / currentSize
uniform float size;
uniform int sceneRowIndex;
uniform int sceneColumnIndex;

in layout(location = 0) vec2 reletivePosition;
in layout(location = 1) vec2 texCoords;
in layout(location = 2) float row;
in layout(location = 3) float column;
in layout(location = 4) uint highlight;

out vec2 vTexCoords;
out uint vHighlight;

void main() {
	vTexCoords = texCoords;

	float actualRow = 0;
	float actualColumn = column - sceneColumnIndex;

	if (row > -1.0) 
	{
		actualRow = row - sceneRowIndex;
		actualRow += 2.0;
	}

	actualRow += 0.5;

	// calculate position
	vec2 position = reletivePosition * 0.1 * size;
	position.y += actualRow * 0.1 * size;
	position.x += actualColumn * 0.1 * size; 

	// const pixel size conversions
	vec2 constPixel = idealRatio * position;

	// screen space conversions
	vec2 screenSpace = constPixel * 2 - 1;
	screenSpace.y *= -1;

	gl_Position = vec4(screenSpace, 0.0, 1.0);
}