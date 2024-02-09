#pragma once

#include <Image/Image.h>


class Texture
{
private:
    int _width, _height;
    unsigned int _rendererID;

    void _initTexture(const Pixel fill, const int width, const int height);
    void _initTexture(const Image* image);

public:
    Texture()
      : _width(1),
        _height(1),
        _rendererID(0)
    {
        _initTexture(Pixel(0.0f, 0.0f, 0.0f, 0.0f), _width, _height);
    }
    Texture(const int width, const int height)
      : _width(width),
        _height(height),
        _rendererID(0)
    {}
    Texture(const Image* image);

    int getWidth() const noexcept;
    int getHeight() const noexcept;
    unsigned int getID() const noexcept;

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    void reInitTexture(const Pixel fill, int width, int height);
    void reInitTexture(const Pixel* pixels, int width, int height);
    void reInitTexture(const Image* image);
};
