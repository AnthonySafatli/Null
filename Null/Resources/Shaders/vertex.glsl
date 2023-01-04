// VERTEX
#version 330 core

uniform vec2 idealRatio;
// uniform float aspectRatioCalcs; // calcs for aspect ratio stuff
uniform float size;
uniform int sceneRowIndex;

in layout(location = 0) vec2 position;
in layout(location = 1) vec2 texCoords;
in layout(location = 2) float row;
in layout(location = 3) float column;

out vec2 vTexCoords;

void main() {
	vTexCoords = texCoords;

	// calculate position
	vec2 newPosition = position * 0.1 * size;
	newPosition.y += row * 0.1 * size;
	newPosition.x += column * 0.1 * size; 

	// const pixel size conversions
	vec2 constPixel = idealRatio * newPosition;

	// screen space conversions
	vec2 screenSpace = constPixel * 2 - 1;
	screenSpace.y *= -1;

	gl_Position = vec4(screenSpace, 0.0, 1.0);
}