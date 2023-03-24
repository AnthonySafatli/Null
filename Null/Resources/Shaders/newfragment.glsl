// FRAGMENT
#version 330 core

const uint INVISIBLE = 0;
const uint NORMAL = 1;
const uint COMMAND_LINE = 2;
const uint STATUS_TEXT = 3;

uniform vec4 foreground;
uniform sampler2D tex;

in vec2 vTexCoords;
in float vRow;
flat in uint vType;

out vec4 color;

void main() {
    color = texture(tex, vTexCoords) * foreground;

    if (vType == INVISIBLE)
        color = vec4(0);
    else if (vType == STATUS_TEXT)
        color.w *= 0.8;
}