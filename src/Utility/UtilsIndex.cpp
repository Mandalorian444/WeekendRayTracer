#include "UtilsIndex.h"

int IndexFrom2D(const int x, const int y, const int width)
{
    return x + y * width;
}

int IndexFrom2D(const Coord2D coord, const int width)
{
    return IndexFrom2D(coord._x, coord._y, width);
}

Coord2D IndexTo2D(const int width, const int height, const int index)
{
    return {index % width, index / width};
}
