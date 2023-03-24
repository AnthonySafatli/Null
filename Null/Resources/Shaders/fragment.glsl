// FRAGMENT
#version 330 core

#define INVISIBLE 0.0
#define NORMAL    1.0
#define CURSOR    2.0
#define STATUS    3.0
#define COMMAND   4.0
#define MARGIN    5.0

uniform vec4 foreground;
uniform sampler2D tex;

in vec2 vTexCoords;
in float vRow;
in float vType;

out vec4 color;

void main() {
    color = texture(tex, vTexCoords) * foreground;

    if (vType == INVISIBLE)
        color = vec4(0);

    if (vType == CURSOR)
        color = foreground;

    if (vType == MARGIN || vType == STATUS)
        color.w *= 0.6;
}