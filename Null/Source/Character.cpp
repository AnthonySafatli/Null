#include <iostream>

#include "Headers/Character.h"

TexCoords::TexCoords(float u, float v) : u(u), v(v)
{}

TexCoords GetCoords(char ch)
{
    int index = TexIndex[ch - 33];
    int x = index % 10 - 1;
    int y = index / 10; 

    return TexCoords((float)x / 10.0, (float)y / 10.0);
}
