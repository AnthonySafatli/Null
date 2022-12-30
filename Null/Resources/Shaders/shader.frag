// FRAGMENT
#version 330 core

uniform sampler2D tex;
uniform vec4 backgroundColour;

in vec2 vTexCoords;

out vec4 color;

void main() {
	vec4 texColour = texture(tex, vTexCoords);
	if (texColour.w == 0.0) 
		texColour = backgroundColour;

	color = texColour;
}