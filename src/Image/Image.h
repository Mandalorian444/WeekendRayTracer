#pragma once

#include <vector>


struct Pixel
{
    float _r, _g, _b, _a;

    constexpr Pixel(const float r, const float g, const float b, const float a)
      : _r(r),
        _g(g),
        _b(b),
        _a(a)
    {}
};

class Image
{
private:
    int _width, _height;
    std::vector<Pixel> _pixels;

public:
    constexpr Image()
      : _width(720),
        _height(480),
        _pixels((720 * 480), {0, 0, 0, 0})
    {}
    constexpr Image(
        const int width, const int height, const Pixel fill = {0, 0, 0, 0}
    )
      : _width(width),
        _height(height),
        _pixels((width * height), fill)
    {}

    const int getWidth() const noexcept { return _width; }
    const int getHeight() const noexcept { return _height; }
    const std::vector<Pixel>& getPixels() const;
    std::vector<Pixel>& getPixels();
};
