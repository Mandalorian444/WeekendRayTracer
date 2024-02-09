#include "Image.h"

const std::vector<Pixel>& Image::getPixels() const { return _pixels; }

std::vector<Pixel>& Image::getPixels()
{
    return const_cast<std::vector<Pixel>&>(std::as_const(*this).getPixels());
}
