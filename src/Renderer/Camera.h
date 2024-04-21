#pragma once

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
    Camera()
      : _origin(0.0f, 0.0f, 0.0f),
        _lower_left_corner(-2.0f, -1.5f, -1.0f),
        _horizontal(4.0f, 0.0f, 0.0f),
        _vertical(0.0f, 3.0f, 0.0f)
    {}
    inline Ray get_ray(float u, float v)
    {
        return Ray(
            _origin,
            _lower_left_corner + u * _horizontal + v * _vertical - _origin
        );
    }
};
