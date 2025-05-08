#pragma once

#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>


#include <Renderer/Camera.h>
#include <Renderer/HitPrimitive.h>
#include <Renderer/HitableList.h>
#include <Renderer/Material.h>
#include <Renderer/Ray.h>
#include <Renderer/Sphere.h>


class ChunkData
{
    std::size_t _total;
    const std::size_t _singleSize;
    std::mutex _mutex;

public:
    ChunkData(const std::size_t singleSize = 64u)
      : _total(0),
        _singleSize(singleSize)
    {}
    //  Increments the total and returns the pre-incremented total
    //  No direct function to get the total is defined to enforce thread safety
    [[nodiscard]] const std::size_t increment()
    {
        std::lock_guard lg(_mutex);
        const auto ret = _total;
        _total += _singleSize;
        return ret;
    }
    [[nodiscard]] std::size_t getChunkSize() const { return _singleSize; }
};

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

Hitable* random_scene(
    std::uniform_real_distribution<float>& distribution, std::mt19937& gen
)
{
    const int n    = 500;
    Hitable** list = new Hitable*[n + 1];

    list[0] = new Sphere(
        Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5f, 0.5f, 0.5f))
    );

    int i = 1;

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            float choose_mat = distribution(gen);
            Vec3 center(
                a + 0.9f * distribution(gen), 0.2f, b + 0.9f * distribution(gen)
            );

            if ((center - Vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
            {
                if (choose_mat < 0.8f)
                {
                    list[i++] = new Sphere(
                        center,
                        0.2f,
                        new Lambertian(Vec3(
                            distribution(gen) * distribution(gen),
                            distribution(gen) * distribution(gen),
                            distribution(gen) * distribution(gen)
                        ))
                    );
                }
                else if (choose_mat < 0.95f)
                {
                    list[i++] = new Sphere(
                        center,
                        0.2f,
                        new Metal(Vec3(
                            0.5f * (1.0f + distribution(gen)),
                            0.5f * (1.0f + distribution(gen)),
                            0.5f * (1.0f + distribution(gen))
                        ))
                    );
                }
                else
                {
                    list[i++] = new Sphere(center, 0.2f, new Dielectric(1.5f));
                }
            }
        }
    }

    list[i++] = new Sphere(Vec3(0, 1, 0), 1.0f, new Dielectric(1.5f));
    list[i++] = new Sphere(
        Vec3(-4, 1, 0), 1.0f, new Lambertian(Vec3(0.4f, 0.2f, 0.1f))
    );
    list[i++] = new Sphere(
        Vec3(4, 1, 0), 1.0f, new Metal(Vec3(0.7f, 0.6f, 0.5f), 0.0f)
    );

    return new HitableList(list, i);
}

void RenderScene(
    Image& image,
    const std::size_t samples   = 2u,
    const std::size_t chunkSize = 4096u,
    bool test                   = false
)
{
    const int width  = image.getWidth();
    const int height = image.getHeight();
    auto& pixels     = image.getPixels();

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
    Hitable* world       = new HitableList(list, objectCount);
    Hitable* randomScene = random_scene(distribution, gen);

    Vec3 lookFrom(16.0f, 2.0f, 6.0f);
    Vec3 lookAt(0.0f, 0.0f, 0.5f);
    float dist_to_focus = (lookFrom - lookAt).length();
    float aperture      = 0.1f;

    Camera cam(
        lookFrom,
        lookAt,
        Vec3(0.0f, 1.0f, 0.0f),
        15.0f,
        static_cast<float>(width) / height,
        aperture,
        dist_to_focus
    );

    const auto now = std::chrono::system_clock::now();

    if (!test)
    {
        std::cout << "Using std::transform\n";
        const auto renderPixels = [&](const Pixel& current)
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
                const Ray r{cam.get_ray(u, v, distribution, gen)};
                const Vec3 point = r.point_at_parameter(2.0f);
                color += Color(r, randomScene, 0, distribution, gen);
            }
            color /= static_cast<float>(samples);
            return Pixel{color.x(), color.y(), color.z(), 1.0f};
        };

        std::transform(
            std::execution::par,
            pixels.begin(),
            pixels.end(),
            pixels.begin(),
            renderPixels
        );
    }

    else
    {
        std::cout << "My own threads\n";
        const auto renderPixelChunk =
            [&](ChunkData& chunk,
                const std::vector<Pixel>::iterator& pixelsBegin,
                const std::size_t pixelsSize,
                const int width,
                const int height)
        {
            std::size_t totalChunk = chunk.increment();
            while (totalChunk < pixelsSize)
            {
                std::vector<Pixel>::iterator chunkBegin =
                    pixelsBegin + totalChunk;
                const std::size_t endIdx = totalChunk + chunk.getChunkSize();
                const std::vector<Pixel>::iterator& chunkEnd =
                    endIdx < pixelsSize ? pixelsBegin + endIdx
                                        : pixelsBegin + pixelsSize;
                for (; chunkBegin != chunkEnd; ++chunkBegin)
                {
                    const std::size_t index =
                        std::distance(pixelsBegin, chunkBegin);
                    const Coord2D uv = IndexTo2D(width, height, index);
                    Vec3 color(0.0f, 0.0f, 0.0f);
                    for (std::size_t i = 0u; i < samples; ++i)
                    {
                        const Ray r{cam.get_ray(
                            (uv._x + distribution(gen)) / width,
                            (uv._y + distribution(gen)) / height,
                            distribution,
                            gen
                        )};
                        const Vec3 point = r.point_at_parameter(2.0f);
                        color += Color(r, randomScene, 0, distribution, gen);
                    }
                    color /= static_cast<float>(samples);
                    *chunkBegin = Pixel{color.x(), color.y(), color.z(), 1.0f};
                }
                totalChunk = chunk.increment();
            }
        };

        ChunkData chunkData(chunkSize);
        const std::size_t threadCount = std::thread::hardware_concurrency();
        std::cout << "Threads: " << threadCount << std::endl;
        std::vector<std::thread> threads;
        threads.reserve(threadCount);
        for (std::size_t i = 0u; i < threadCount; ++i)
        {
            std::cout << "Launching thread " << i << std::endl;
            threads.push_back(std::thread(
                renderPixelChunk,
                std::ref(chunkData),
                pixels.begin(),
                pixels.size(),
                width,
                height
            ));
        }
        for (auto& t : threads)
        {
            t.join();
        }
        threads.clear();
    }

    std::cout << "Render time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::system_clock::now() - now
                 )
                     .count()
              << " ms" << std::endl;
}
