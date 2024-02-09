#include "GenUV.h"

#include <algorithm>

#include <Utility/UtilsIndex.h>

void GenerateUV(Image& image)
{
    const int width  = image.getWidth();
    const int height = image.getHeight();
    auto& pixels      = image.getPixels();

    std::transform(
        pixels.begin(),
        pixels.end(),
        pixels.begin(),
        [&](const Pixel& current)
        {
            const int index = &current - &(*pixels.begin());
            const float r =
                static_cast<float>(IndexTo2D(width, height, index)._x) / width;
            const float g =
                static_cast<float>(IndexTo2D(width, height, index)._y) / height;
            return Pixel{r, g, 0.25f, 1.0f};
        }
    );
}
