#pragma once

#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <random>


#include <Renderer/Camera.h>
#include <Renderer/HitPrimitive.h>
#include <Renderer/HitableList.h>
#include <Renderer/Material.h>
#include <Renderer/Ray.h>
#include <Renderer/Sphere.h>


Vec3 Color(const Ray& r)
{
    float t = HitSphere(r, Vec3(0.0f, 0.0f, -1.0f), 0.5f);
    if (t > 0.0)
    {
        Vec3 normal =
            Unit_vector(r.point_at_parameter(t) - Vec3(0.0f, 0.0f, -1.0f));
        return 0.5f * (normal + Vec3(1.0f, 1.0f, 1.0f));
    }
    Vec3 unit_direction = Unit_vector(r.direction());
    t                   = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

Vec3 Color(
    const Ray& r,
    Hitable* world,
    int depth,
    std::uniform_real_distribution<float>& distribution,
    std::mt19937& gen
)
{
    HitRecord rec;
    if (world->hit(r, 0.0001f, std::numeric_limits<float>().max(), rec))
    {
        Ray scattered;
        Vec3 attenuation;
        if (depth < 50 && rec.material->scatter(
                              r, rec, attenuation, scattered, distribution, gen
                          ))
        {
            return attenuation *
                   Color(scattered, world, depth + 1, distribution, gen);
        }
        else
        {
            return Vec3(0.0f, 0.0f, 0.0f);
        }
    }
    Vec3 unit_direction = Unit_vector(r.direction());
    float t             = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

void RenderScene(Image& image)
{
    const int width  = image.getWidth();
    const int height = image.getHeight();
    auto& pixels     = image.getPixels();

    const int samples = 50;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    //  Dimensions created by vectors need to be of the same aspect ratio as the
    //  image
    Vec3 lower_left_corner(-2.0f, -1.5f, -1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 3.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);

    constexpr const int objectCount = 5;
    Hitable* list[objectCount];
    list[0] = new Sphere(
        Vec3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(Vec3(0.1f, 0.2f, 0.5f))
    );
    list[1] = new Sphere(
        Vec3(0.0f, -100.5f, -1.0f),
        100.0f,
        new Lambertian(Vec3(0.8f, 0.8f, 0.0f))
    );
    list[2] = new Sphere(
        Vec3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(Vec3(0.8f, 0.6f, 0.2f), 1.0f)
    );
    list[3] = new Sphere(Vec3(-1.0f, 0.0f, -1.0f), 0.5f, new Dielectric(1.5f));
    list[4] =
        new Sphere(Vec3(-1.0f, 0.0f, -1.0f), -0.45f, new Dielectric(1.5f));
    Hitable* world = new HitableList(list, objectCount);
    Camera cam(
        Vec3(-2.0f, 2.0f, 1.0f),
        Vec3(0.0f, 0.0f, -1.0f),
        Vec3(0.0f, 1.0f, 0.0f),
        30.0f,
        static_cast<float>(width) / height
    );

    const auto now = std::chrono::system_clock::now();

    std::transform(
        std::execution::par,
        pixels.begin(),
        pixels.end(),
        pixels.begin(),
        [&](const Pixel& current)
        {
            Vec3 color(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < samples; ++i)
            {
                const int index = &current - &(*pixels.begin());
                const float u =
                    static_cast<float>(
                        IndexTo2D(width, height, index)._x + distribution(gen)
                    ) /
                    width;
                const float v =
                    static_cast<float>(
                        IndexTo2D(width, height, index)._y + distribution(gen)
                    ) /
                    height;
                const Ray r{cam.get_ray(u, v)};
                const Vec3 point = r.point_at_parameter(2.0f);
                color += Color(r, world, 0, distribution, gen);
            }
            color /= static_cast<float>(samples);
            return Pixel{color.x(), color.y(), color.z(), 1.0f};
        }
    );

    std::cout << "Render time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now() - now
                 )
                     .count()
              << " ms" << std::endl;
}
