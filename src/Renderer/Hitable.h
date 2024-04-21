#pragma once

#include <Renderer/Ray.h>
#include <Utility/Vector.h>


struct HitRecord
{
    float t;
    Vec3 point;
    Vec3 normal;
};

class Hitable
{
public:
    virtual bool
    hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
};
