// FRAGMENT
#version 330 core

uniform sampler2D tex;

in vec2 vTexCoords;
flat in uint vHighlight;

out vec4 color;

void main() {
	if (vHighlight == 0u)
		color = texture(tex, vTexCoords);
	else 
		color = vec4(1.0, 0.0, 0.0, 1.0); // TODO: get fixed
}