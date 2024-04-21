#pragma once

#include <Renderer/Hitable.h>


class HitableList : public Hitable
{
public:
    Hitable** _list;
    int _size;

public:
    HitableList() = default;
    HitableList(Hitable** l, int n)
      : _list(l),
        _size(n)
    {}

    virtual bool
    hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const override;
};

bool HitableList::hit(const Ray& r, float t_min, float t_max, HitRecord& rec)
    const
{
    HitRecord temp_rec;
    bool hit_anything   = false;
    auto closest_so_far = t_max;

    for (int i = 0; i < _size; i++)
    {
        if (_list[i]->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything   = true;
            closest_so_far = temp_rec.t;
            rec            = temp_rec;
        }
    }

    return hit_anything;
}
