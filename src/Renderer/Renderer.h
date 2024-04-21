#pragma once

#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>


#include <Renderer/HitPrimitive.h>
#include <Renderer/HitableList.h>
#include <Renderer/Sphere.h>
#include <Renderer/Ray.h>


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

Vec3 Color(const Ray& r, Hitable* world)
{
    HitRecord rec;
    if (world->hit(r, 0.0f, std::numeric_limits<float>().max(), rec))
    {
        return 0.5f * (rec.normal + Vec3(1.0f, 1.0f, 1.0f));
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

    //  Dimensions created by vectors need to be of the same aspect ratio as the
    //  image
    Vec3 lower_left_corner(-2.0f, -1.5f, -1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 3.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);

    Hitable *list[2];
    list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f);
    Hitable *world = new HitableList(list, 2);

    const auto now = std::chrono::system_clock::now();

    std::transform(
        std::execution::par,
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
            const Ray r(
                origin, lower_left_corner + u * horizontal + v * vertical
            );
            const Vec3 point = r.point_at_parameter(2.0f);
            const Vec3 color = Color(r, world);
            // const Vec3 color = Color(r);
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
