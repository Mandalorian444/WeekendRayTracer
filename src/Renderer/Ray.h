#pragma once

#include <algorithm>

#include <Image/Image.h>
#include <Utility/UtilsIndex.h>
#include <Utility/Vector.h>


class ray
{
public:
    vec3 orig;
    vec3 dir;

public:
    ray() {}
    ray(const vec3& origin, const vec3& direction)
      : orig(origin),
        dir(direction)
    {}
    vec3 origin() const { return orig; }
    vec3 direction() const { return dir; }
    vec3 point_at_parameter(float t) const { return orig + t * dir; }
};

vec3 Color(const ray& r)
{
    vec3 unit_direction = Unit_vector(r.direction());
    float t             = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

void RenderScene(Image& image)
{
    const int width  = image.getWidth();
    const int height = image.getHeight();
    auto& pixels     = image.getPixels();

    vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
    vec3 horizontal(4.0f, 0.0f, 0.0f);
    vec3 vertical(0.0f, 2.0f, 0.0f);
    vec3 origin(0.0f, 0.0f, 0.0f);

    std::transform(
        pixels.begin(),
        pixels.end(),
        pixels.begin(),
        [&](const Pixel& current)
        {
            const int index = &current - &(*pixels.begin());
            const float u =
                static_cast<float>(IndexTo2D(width, height, index)._x) / width;
            const float v =
                static_cast<float>(IndexTo2D(width, height, index)._y) / height;
            const ray r(
                origin, lower_left_corner + u * horizontal + v * vertical
            );
            const vec3 col = Color(r);
            return Pixel{col.x(), col.y(), col.z(), 1.0f};
        }
    );
}
