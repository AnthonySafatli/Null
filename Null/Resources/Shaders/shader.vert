// VERTEX
#version 330 core

uniform vec2 dimensions;
uniform vec2 idealDimensions;
uniform float idealRatio; // idealDimensions / currentDimensions
uniform float aspectRatioCalcs; // calcs for aspect ratio stuff
uniform float size;

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 anchor;
layout (location = 2) in vec2 texCoords;

out vec2 vTexCoords;

void main() {
	vTexCoords = texCoords;

	// scale using size
	vec2 scaled = position - anchor;
	scaled *= size;
	scaled += anchor;

	// const pixel size conversions
	vec2 updatedPos = idealDimensions / dimensions * scaled;

	// screen space conversions
	vec2 screenSpace = updatedPos * 2 - 1;
	screenSpace.y *= -1;

	gl_Position = vec4(screenSpace, 0.0, 1.0);
}