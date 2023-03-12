// FRAGMENT
#version 330 core

const float STATUS_BAR_ROW = -2.0;

uniform vec4 foreground;
uniform sampler2D tex;

in vec2 vTexCoords;
in float vRow;
flat in uint vHighlight;

out vec4 color;

void main() {
	if (vHighlight == 0u)
		color = texture(tex, vTexCoords) * foreground;

	else 
	{
		vec4 result = texture(tex, vTexCoords) * foreground;
		color = vec4(1 - result.x, 1 - result.y, 1 - result.z, 1 - result.w);

		/*
		if (vRow == STATUS_BAR_ROW)
			color *= 0.75;
		*/
	}
}