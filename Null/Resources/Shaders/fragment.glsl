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
	{
		vec4 result = texture(tex, vTexCoords);
		color = vec4(1 - result.x, 1 - result.y, 1 - result.z, 1 - result.w);
	}
}