// FRAGMENT
#version 330 core

#define INVISIBLE        0.0
#define NORMAL           1.0
#define CURSOR           2.0
#define CURSOR_COMMAND   3.0
#define CURSOR_INVISIBLE 4.0
#define STATUS           5.0
#define COMMAND          6.0
#define MARGIN           7.0
#define MARGIN_SELECTED  8.0

uniform vec4 foreground;
uniform int cursorRow;
uniform sampler2D tex;

in vec2 vTexCoords;
in float vRow;
in float vType;

out vec4 color;

void main() {
    color = texture(tex, vTexCoords) * foreground;

    if (vType == INVISIBLE || vType == CURSOR_INVISIBLE)
        color = vec4(0);

    if (vType == CURSOR || vType == CURSOR_COMMAND)
        color = foreground;

    if (vType == STATUS)
        color.w *= 0.7;

    if (vType == MARGIN || vType == MARGIN_SELECTED)
    {
        if (vRow != cursorRow && vType != MARGIN_SELECTED)
            color.w *= 0.4;
    }
}