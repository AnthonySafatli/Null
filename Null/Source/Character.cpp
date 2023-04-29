#include "Headers/Character.h"

#include <iostream>

TexCoords::TexCoords(float u, float v) : u(u), v(v)
{}

TexCoords GetCoords(char ch)
{
    int index = TexIndex[ch - 28];

    int x = index % 10;
    int y = index / 10; 

    return TexCoords((float)x / 10.0, (float)y / 10.0);
}
