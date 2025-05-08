#pragma once

#include <Renderer/Ray.h>


float HitSphere(const Ray& ray, const Vec3& center, float radius/*, float t*/)
{
    Vec3 oc = ray.origin() - center;
    float a = Dot(ray.direction(), ray.direction());
    float b = 2.0f * Dot(oc, ray.direction());
    float c = Dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return -1.0f;
    }
    else
    {
        return (-b - sqrt(discriminant)) / (2.0f * a);
    }
}
