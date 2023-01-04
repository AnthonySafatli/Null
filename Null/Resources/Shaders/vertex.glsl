// VERTEX
#version 330 core

uniform vec2 idealRatio;
// uniform float aspectRatioCalcs; // calcs for aspect ratio stuff
uniform float size;
uniform int sceneRowIndex;

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

	if (row > 0)
		actualRow = row - sceneRowIndex;

	// calculate position
	vec2 position = reletivePosition * 0.1 * size;
	position.y += actualRow * 0.1 * size;
	position.x += column * 0.1 * size; 

	// const pixel size conversions
	vec2 constPixel = idealRatio * position;

	// screen space conversions
	vec2 screenSpace = constPixel * 2 - 1;
	screenSpace.y *= -1;

	gl_Position = vec4(screenSpace, 0.0, 1.0);
}