#pragma once

#include <algorithm>
#include <random>

#include <Image/Image.h>
#include <Utility/UtilsIndex.h>
#include <Utility/Vector.h>


class Ray
{
public:
    Vec3 orig;
    Vec3 dir;

public:
    Ray() {}
    Ray(const Vec3& origin, const Vec3& direction)
      : orig(origin),
        dir(direction)
    {}
    Vec3 origin() const { return orig; }
    Vec3 direction() const { return dir; }
    Vec3 point_at_parameter(float t) const { return orig + t * dir; }
};

Vec3 Random_in_unit_sphere(
    std::uniform_real_distribution<float>& distribution, std::mt19937& gen
)
{
    Vec3 point;
    do
    {
        point =
            2.0f *
                Vec3(distribution(gen), distribution(gen), distribution(gen)) -
            Vec3(1.0f, 1.0f, 1.0f);
    }
    while (point.squared_length() >= 1.0f);
    return point;
}

Vec3 Random_in_unit_disk(
    std::uniform_real_distribution<float>& distribution, std::mt19937& gen
)
{
    Vec3 p;
    do
    {
        p = 2.0f * Vec3(distribution(gen), distribution(gen), 0) -
            Vec3(1, 1, 0);
    }
    while (Dot(p, p) >= 1.0f);
    return p;
}
