// VERTEX
#version 330 core

uniform vec2 dimensions;
uniform vec2 idealDimensions;

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 vTexCoords;

void main() {
	vTexCoords = texCoords;

	// const pixel size conversions
	vec2 updatedPos = idealDimensions / dimensions * position;

	//screen space conversions
	vec2 finalPos = updatedPos * 2 - 1;
	finalPos.y *= -1;

	gl_Position = vec4(finalPos, 0.0, 1.0);
}