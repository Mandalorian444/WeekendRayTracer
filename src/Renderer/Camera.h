#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <Renderer/Ray.h>
#include <Utility/Vector.h>


class Camera
{
public:
    Vec3 _origin;
    Vec3 _lower_left_corner;
    Vec3 _horizontal;
    Vec3 _vertical;

public:
    Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vUp, float vfov_deg, float aspect_ratio)
      : _origin(lookFrom),
        _lower_left_corner(-2.0f, -1.5f, -1.0f),
        _horizontal(4.0f, 0.0f, 0.0f),
        _vertical(0.0f, 3.0f, 0.0f)
    {
        float theta        = vfov_deg * M_PI / 180.0f;
        float half_height  = tan(theta / 2.0f);
        float half_width   = aspect_ratio * half_height;
        Vec3 w = Unit_vector(lookFrom - lookAt);
        Vec3 u = Unit_vector(Cross(vUp, w));
        Vec3 v = Cross(w, u);
        _lower_left_corner = _origin - half_width * u - half_height * v - w;
        _horizontal        = (2.0f * half_width * u);
        _vertical          = (2.0f * half_height * v);
    }

    inline Ray get_ray(float u, float v)
    {
        return Ray(
            _origin,
            _lower_left_corner + u * _horizontal + v * _vertical - _origin
        );
    }
};
