#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <random>

#include <Renderer/Ray.h>
#include <Utility/Vector.h>


class Camera
{
public:
    Vec3 _origin;
    Vec3 _lower_left_corner;
    Vec3 _horizontal;
    Vec3 _vertical;
    Vec3 _w, _u, _v;
    float _lens_radius;

public:
    Camera(
        Vec3 lookFrom,
        Vec3 lookAt,
        Vec3 vUp,
        float vfov_deg,
        float aspect_ratio,
        float aperture,
        float focus_dist
    )
      : _origin(lookFrom),
        _lower_left_corner(0.0f, 0.0f, 0.0f),
        _horizontal(0.0f, 0.0f, 0.0f),
        _vertical(0.0f, 0.0f, 0.0f),
        _w(Unit_vector(lookFrom - lookAt)),
        _u(Unit_vector(Cross(vUp, _w))),
        _v(Cross(_w, _u)),
        _lens_radius(aperture / 2.0f)
    {
        float theta        = vfov_deg * M_PI / 180.0f;
        float half_height  = tan(theta / 2.0f);
        float half_width   = aspect_ratio * half_height;
        _lower_left_corner = _origin - half_width * focus_dist * _u -
                             half_height * focus_dist * _v - focus_dist * _w;
        _horizontal = (2.0f * half_width * focus_dist * _u);
        _vertical   = (2.0f * half_height * focus_dist * _v);
    }

    inline Ray get_ray(
        float u,
        float v,
        std::uniform_real_distribution<float>& dist,
        std::mt19937& gen
    )
    {
        Vec3 rd     = _lens_radius * Random_in_unit_disk(dist, gen);
        Vec3 offset = _u * rd.x() + _v * rd.y();
        return Ray(
            _origin + offset,
            _lower_left_corner + u * _horizontal + v * _vertical - _origin -
                offset
        );
    }
};
