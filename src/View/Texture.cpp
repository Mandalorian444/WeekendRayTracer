#include "Texture.h"

#include <cassert>

#include <View/GLHelpers.h>
#include <gl/glew.h>


void Texture::_initTexture(const Pixel fill, int width, int height)
{
    GLCall(glGenTextures(1, &_rendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, _rendererID));

    // following are required to show anything on screen
    // set scale-down filter
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    // set scale-up filter
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    // set S (x) wrap setting
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER)
    );
    // set T (y) wrap setting
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER)
    );

    reInitTexture(fill, width, height);
    unbind();
}

void Texture::_initTexture(const Image* image)
{
    GLCall(glGenTextures(1, &_rendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, _rendererID));

    // following are required to show anything on screen
    // set scale-down filter
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    // set scale-up filter
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    // set S (x) wrap setting
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER)
    );
    // set T (y) wrap setting
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER)
    );

    reInitTexture(&image->getPixels()[0], _width, _height);
    unbind();
}

Texture::Texture(const Image* image)
  : _width(image->getWidth()),
    _height(image->getHeight()),
    _rendererID(0)
{
    _initTexture(image);
}

int Texture::getWidth() const noexcept { return _width; }

int Texture::getHeight() const noexcept { return _height; }

unsigned int Texture::getID() const noexcept { return _rendererID; }

void Texture::bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, _rendererID));
}

void Texture::unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }

void Texture::reInitTexture(const Pixel fill, int width, int height)
{
    bind();
    assert(width > 0 && height > 0);
    _width  = width;
    _height = height;
    std::vector<Pixel> blankImage((_width * _height), fill);
    GLCall(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA32F,
        _width,
        _height,
        0,
        GL_RGBA,
        GL_FLOAT,
        (void*)&blankImage[0]._r
    ));
}

void Texture::reInitTexture(const Pixel* pixels, int width, int height)
{
    bind();
    bool blank = false;
    if (pixels != nullptr)
    {
        if (width <= 0 || height <= 0)
        {
            blank   = true;
        }
        else
        {
            _width  = width;
            _height = height;
            GLCall(glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA32F,
                _width,
                _height,
                0,
                GL_RGBA,
                GL_FLOAT,
                (void*)&pixels->_r
            ));
        }
    }
    else
    {
        blank = true;
    }

    if (blank)
    {
        std::vector<float> blankImage((_width * _height), 0.0f);
        GLCall(glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA32F,
            _width,
            _height,
            0,
            GL_RGBA,
            GL_FLOAT,
            (void*)&blankImage[0]
        ));
    }
}

void Texture::reInitTexture(const Image* image)
{
    reInitTexture(
        &image->getPixels()[0], image->getWidth(), image->getHeight()
    );
}
