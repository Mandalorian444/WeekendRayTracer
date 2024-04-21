#pragma once

#include <algorithm>

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
