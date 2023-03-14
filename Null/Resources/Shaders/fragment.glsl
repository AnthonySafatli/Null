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
    color = texture(tex, vTexCoords) * foreground;

    if (vHighlight != 0u)
        color = foreground;
}