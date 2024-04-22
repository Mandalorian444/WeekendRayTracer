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

bool Refract(
    const Vec3& v, const Vec3& normal, float ni_over_nt, Vec3& refracted
)
{
    Vec3 uv            = Unit_vector(v);
    float dt           = Dot(uv, normal);
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted =
            ni_over_nt * (uv - normal * dt) - normal * sqrt(discriminant);
        return true;
    }
    else
        return false;
}

float Schlick(float cosine, float refractiveIndex)
{
    float r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
    r0       = r0 * r0;
    return r0 + (1 - r0) * powf((1 - cosine), 5);
};

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

class Dielectric : public Material
{
public:
    float _refractiveIndex;

public:
    Dielectric(float refractiveIndex)
      : _refractiveIndex(refractiveIndex)
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
        Vec3 outwardNormal;
        Vec3 reflected =
            Reflect(Unit_vector(rayIn.direction()), hitRecord.normal);
        float ni_over_nt;
        attenuation = Vec3(1.0f, 1.0f, 1.0f);
        Vec3 refracted;
        float reflectProb;
        float cosine;
        if (Dot(rayIn.direction(), hitRecord.normal) > 0)
        {
            outwardNormal = -hitRecord.normal;
            ni_over_nt    = _refractiveIndex;
            cosine        = _refractiveIndex *
                     Dot(rayIn.direction(), hitRecord.normal) /
                     rayIn.direction().length();
        }
        else
        {
            outwardNormal = hitRecord.normal;
            ni_over_nt    = 1.0f / _refractiveIndex;
            cosine        = -Dot(rayIn.direction(), hitRecord.normal) /
                     rayIn.direction().length();
        }

        if (Refract(rayIn.direction(), outwardNormal, ni_over_nt, refracted))
        {
            reflectProb = Schlick(cosine, _refractiveIndex);
        }
        else
        {
            reflectProb = 1.0f;
        }

        if (distribution(gen) < reflectProb)
        {
            scattered = Ray(hitRecord.point, reflected);
        }
        else
        {
            scattered = Ray(hitRecord.point, refracted);
        }
        return true;
    };
};
