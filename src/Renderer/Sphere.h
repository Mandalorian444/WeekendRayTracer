#pragma once

#include <Renderer/Hitable.h>
#include <Renderer/Material.h>


class Sphere : public Hitable
{
public:
    Vec3 _center;
    float _radius;
    Material* _material;

public:
    Sphere(){};
    Sphere(Vec3 center, float radius, Material* material)
      : _center(center),
        _radius(radius),
        _material(material)
    {}
    virtual bool
    hit(const Ray& ray, float tMin, float tMax, HitRecord& record
    ) const override;
};

bool Sphere::hit(const Ray& ray, float tMin, float tMax, HitRecord& record)
    const
{
    Vec3 oc            = ray.origin() - _center;
    float a            = ray.direction().squared_length();
    float half_b       = Dot(oc, ray.direction());
    float c            = oc.squared_length() - _radius * _radius;
    float discriminant = half_b * half_b - a * c;
    if (discriminant > 0)
    {
        float root = sqrt(discriminant);
        float temp = (-half_b - root) / a;
        if (temp < tMax && temp > tMin)
        {
            record.t        = temp;
            record.point    = ray.point_at_parameter(record.t);
            record.normal   = (record.point - _center) / _radius;
            record.material = _material;
            return true;
        }
        temp = (-half_b + root) / a;
        if (temp < tMax && temp > tMin)
        {
            record.t        = temp;
            record.point    = ray.point_at_parameter(record.t);
            record.normal   = (record.point - _center) / _radius;
            record.material = _material;
            return true;
        }
    }
    return false;
}
