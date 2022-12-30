// FRAGMENT
#version 330 core

uniform sampler2D tex;

in vec2 vTexCoords;

out vec4 color;

void main() {
	// color = texture(tex, vTexCoords);
	color = vec4(1.0, 1.0, 1.0, 1.0);
}