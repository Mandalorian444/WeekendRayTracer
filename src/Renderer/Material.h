#pragma once

#include <Renderer/Hitable.h>
#include <Renderer/Ray.h>
#include <Utility/Vector.h>


class Material
{
public:
    virtual bool scatter(
        const Ray& rayIn,
        const HitRecord& hitRecord,
        Vec3& attenuation,
        Ray& scattered,
        std::uniform_real_distribution<float>& distribution,
        std::mt19937& gen
    ) const = 0;
};

Vec3 Reflect(const Vec3& v, const Vec3& normal)
{
    return v - 2 * Dot(v, normal) * normal;
}

class Lambertian : public Material
{
public:
    Vec3 _albedo;

public:
    Lambertian(const Vec3& albedo)
      : _albedo(albedo)
    {}

    virtual bool scatter(
        const Ray& rayIn,
        const HitRecord& hitRecord,
        Vec3& attenuation,
        Ray& scattered,
        std::uniform_real_distribution<float>& distribution,
        std::mt19937& gen
    ) const override
    {
        Vec3 target = hitRecord.point + hitRecord.normal +
                      Random_in_unit_sphere(distribution, gen);
        scattered   = Ray(hitRecord.point, target - hitRecord.point);
        attenuation = _albedo;
        return true;
    }
};

class Metal : public Material
{
public:
    Vec3 _albedo;
    float _fuzz;

public:
    Metal(const Vec3& albedo, float fuzz = 0.0f)
      : _albedo(albedo),
        _fuzz(fuzz < 1.0f ? fuzz : 1.0f)
    {}
    virtual bool scatter(
        const Ray& rayIn,
        const HitRecord& hitRecord,
        Vec3& attenuation,
        Ray& scattered,
        std::uniform_real_distribution<float>& distribution,
        std::mt19937& gen
    ) const override
    {
        Vec3 reflected =
            Reflect(Unit_vector(rayIn.direction()), hitRecord.normal);
        scattered =
            Ray(hitRecord.point,
                reflected + _fuzz * Random_in_unit_sphere(distribution, gen));
        attenuation = _albedo;
        return (Dot(scattered.direction(), hitRecord.normal) > 0);
    };
};
