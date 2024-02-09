#pragma once


struct Coord2D
{
    int _x, _y;

    constexpr Coord2D(const int x, const int y)
      : _x(x),
        _y(y)
    {}
};

int IndexFrom2D(const int x, const int y, const int width);
int IndexFrom2D(const Coord2D coord, const int width);
Coord2D IndexTo2D(const int width, const int height, const int index);
