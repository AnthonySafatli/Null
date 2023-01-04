// FRAGMENT
#version 330 core

uniform sampler2D tex;

in vec2 vTexCoords;
in uint vHighlight;

out vec4 color;

void main() {
	color = texture(tex, vTexCoords);
}